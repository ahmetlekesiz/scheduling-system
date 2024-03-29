       #include "binomial.h"
       #include "fatal.h"
       #include <math.h>


       /* START: fig6_52.txt */
        typedef struct BinNode *Position;

        struct BinNode
        {
		    ElementType Item;
            //model_node_type mn;
            Position    LeftChild;
            Position    NextSibling;
        };

/*        struct Collection
        {
            int CurrentSize;
            BinTree TheTrees[ MaxTrees ];
        };*/

        BinQueue
        Initialize( void )
        {
            BinQueue H;
            int i;

            H = malloc( sizeof( struct Collection ) );
            if( H == NULL )
                FatalError( "Out of space!!!" );
            H->CurrentSize = 0;
            for( i = 0; i < MaxTrees; i++ )
                H->TheTrees[ i ] = NULL;
            return H;
        }

        static void
        DestroyTree( BinTree T )
        {
            if( T != NULL )
            {
                DestroyTree( T->LeftChild );
                DestroyTree( T->NextSibling );
                free( T );
            }
        }

        void
        Destroy( BinQueue H )
        {
            int i;

            for( i = 0; i < MaxTrees; i++ )
                DestroyTree( H->TheTrees[ i ] );
        }

        BinQueue
        MakeEmpty( BinQueue H )
        {
            int i;

            Destroy( H );
            for( i = 0; i < MaxTrees; i++ )
                H->TheTrees[ i ] = NULL;
            H->CurrentSize = 0;

            return H;
        }

        /* Not optimized for O(1) amortized performance */
        BinQueue
        Insert( ElementType Item, /*model_node_type m,*/ BinQueue H )
        {
            BinTree NewNode;
            BinQueue OneItem;
			int i;

            NewNode = malloc( sizeof( struct BinNode ) );
            if( NewNode == NULL )
                FatalError( "Out of space!!!" );
            NewNode->LeftChild = NewNode->NextSibling = NULL;
			NewNode->Item = Item;

            OneItem = Initialize( );
            OneItem->CurrentSize = 1;
            OneItem->TheTrees[ 0 ] = NewNode;

            return Merge( H, OneItem );
        }

        ElementType 
        DeleteMin( BinQueue H )
        {
            int i, j;
            int MinTree;   /* The tree with the minimum item */
            BinQueue DeletedQueue;
            Position DeletedTree, OldRoot;
            ElementType MinItem;
			//model_node_type MinItem;

            if( IsEmpty( H ) )
            {
                Error( "Empty binomial queue" );
				MinItem.pValue=-Infinity;
                return MinItem;
            }

            MinItem.pValue = Infinity;
            for( i = 0; i < MaxTrees; i++ )
            {
                if( H->TheTrees[ i ] &&
                    H->TheTrees[ i ]->Item.pValue < MinItem.pValue )
                {
                    /* Update minimum */
                    MinItem = H->TheTrees[ i ]->Item;
					
                    MinTree = i;
                }
            }

            DeletedTree = H->TheTrees[ MinTree ];
            OldRoot = DeletedTree;
            DeletedTree = DeletedTree->LeftChild;
            free( OldRoot );

            DeletedQueue = Initialize( );
            DeletedQueue->CurrentSize = ( 1 << MinTree ) - 1;
            for( j = MinTree - 1; j >= 0; j-- )
            {
                DeletedQueue->TheTrees[ j ] = DeletedTree;
                DeletedTree = DeletedTree->NextSibling;
                DeletedQueue->TheTrees[ j ]->NextSibling = NULL;
            }

            H->TheTrees[ MinTree ] = NULL;
            H->CurrentSize -= DeletedQueue->CurrentSize + 1;

            Merge( H, DeletedQueue );
            return MinItem;
        }

        ElementType
        FindMin( BinQueue H )
        {
            int i;
            ElementType MinItem;

            if( IsEmpty( H ) )
            {
                Error( "Empty binomial queue" );
				MinItem.pValue=0;
                return MinItem;
            }

            MinItem.pValue = Infinity;
			
            for( i = 0; i < MaxTrees; i++ )
            {
                if( H->TheTrees[ i ] &&
                            H->TheTrees[ i ]->Item.pValue < MinItem.pValue ) {
                    MinItem = H->TheTrees[ i ]->Item;
				}	
            }

            return MinItem;
        }

        int
        IsEmpty( BinQueue H )
        {
            return H->CurrentSize == 0;
        }

        int IsFull( BinQueue H )
        {
            return H->CurrentSize == Capacity;
        }

/* START: fig6_54.txt */
        /* Return the result of merging equal-sized T1 and T2 */
        BinTree
        CombineTrees( BinTree T1, BinTree T2 )
        {
            if( T1->Item.pValue > T2->Item.pValue )
                return CombineTrees( T2, T1 );
            T2->NextSibling = T1->LeftChild;
            T1->LeftChild = T2;
            return T1;
        }

        BinQueue
        Merge( BinQueue H1, BinQueue H2 )
        {
            BinTree T1, T2, Carry = NULL;
            int i, j;

            if( H1->CurrentSize + H2->CurrentSize > Capacity )
                Error( "Merge would exceed capacity" );

            H1->CurrentSize += H2->CurrentSize;
            for( i = 0, j = 1; j <= H1->CurrentSize; i++, j *= 2 )
            {
                T1 = H1->TheTrees[ i ]; T2 = H2->TheTrees[ i ];

                switch( !!T1 + 2 * !!T2 + 4 * !!Carry )
                {
                    case 0: /* No trees */
                    case 1: /* Only H1 */
                        break;
                    case 2: /* Only H2 */
                        H1->TheTrees[ i ] = T2;
                        H2->TheTrees[ i ] = NULL;
                        break;
                    case 4: /* Only Carry */
                        H1->TheTrees[ i ] = Carry;
                        Carry = NULL;
                        break;
                    case 3: /* H1 and H2 */
                        Carry = CombineTrees( T1, T2 );
                        H1->TheTrees[ i ] = H2->TheTrees[ i ] = NULL;
                        break;
                    case 5: /* H1 and Carry */
                        Carry = CombineTrees( T1, Carry );
                        H1->TheTrees[ i ] = NULL;
                        break;
                    case 6: /* H2 and Carry */
                        Carry = CombineTrees( T2, Carry );
                        H2->TheTrees[ i ] = NULL;
                        break;
                    case 7: /* All three */
                        H1->TheTrees[ i ] = Carry;
                        Carry = CombineTrees( T1, T2 );
                        H2->TheTrees[ i ] = NULL;
                        break;
                }
            }
            return H1;
        }
		
		// by Borahan T�mer
		BinTree printTree(BinTree p, BinTree *r, int i)
		{
		  BinTree t[20]={NULL}, q; int j;
		  for ( j=0; j<i; j++ ) t[j]= r[j];
		  i=0;
		  if (p!=NULL) { 
		    printf("& %d ",p->Item.id);
			q=p->NextSibling;
			j=0;
			do {
			  while (q!=NULL) { 
			    printf("%d ",q->Item.id);
			    if (q->LeftChild != NULL) { r[i]=q->LeftChild; i++; }
			    q=q->NextSibling;
			  }
			  q=t[j++];
			} while (q!=NULL);
		  }
		  else return NULL;
		  //for (j=0; j<i; j++) t[j]=NULL;
		  printf("\n");
		  printTree(p->LeftChild, r, i);
		}

        BinTree increaseWaitingTimeBinomialTree(BinTree p, BinTree *r, int i, int increment)
       {
           BinTree t[20]={NULL}, q; int j;
           for ( j=0; j<i; j++ ) t[j]= r[j];
           i=0;
           if (p!=NULL) {
               //increase waiting time
               p->Item.wt = p->Item.wt + increment;
               q=p->NextSibling;
               j=0;
               do {
                   while (q!=NULL) {
                       //increase waiting time
                       q->Item.wt = q->Item.wt + increment;
                       if (q->LeftChild != NULL) { r[i]=q->LeftChild; i++; }
                       q=q->NextSibling;
                   }
                   q=t[j++];
               } while (q!=NULL);
           }
           else return NULL;
           increaseWaitingTimeBinomialTree(p->LeftChild, r, i, increment);
       }

        void increaseWaitingTime(BinQueue H, int i, int increment){
            BinTree p2, r2[20]={NULL};
            for (int i = 0; i < 12 ; ++i) {
                p2=H->TheTrees[i];
                increaseWaitingTimeBinomialTree(p2, r2, i, increment);
            }
        };

       double calculateC(ElementType *node, int eI, int eMax){
           if(node->bhCounter == 0){
               return 1;
           }else{
               double top = 2*eI;
               double bottom = 3*eMax;
               double division = top/bottom;
               double calculation = -pow(division, 3);
               return 1/exp(calculation);
           }
       }

       double calculatePV(ElementType *node, int e, int t, int eMax){
           //calculate the priority value
           //  check if there is a process that has the same e value
           //      if exist take tArrival
           //      else c(eI)*eI (Implement the c(eI) for first insertion and further insertions).
           //check if there are any same eI
           return calculateC(node, e, eMax)*e;
       }

        BinTree treeTraversal(BinTree p, BinTree *r, int i, int eMax)
       {
           BinTree t[20]={NULL}, q; int j;
           for ( j=0; j<i; j++ ) t[j]= r[j];
           i=0;
           if (p!=NULL) {
               p->Item.pValue = calculatePV(&p->Item, p->Item.e, p->Item.t, eMax);
               q=p->NextSibling;
               j=0;
               do {
                   while (q!=NULL) {
                       q->Item.pValue = calculatePV(&q->Item, q->Item.e, q->Item.t, eMax);
                       if (q->LeftChild != NULL) { r[i]=q->LeftChild; i++; }
                       q=q->NextSibling;
                   }
                   q=t[j++];
               } while (q!=NULL);
           }
           else return NULL;
           treeTraversal(p->LeftChild, r, i, eMax);
       }

