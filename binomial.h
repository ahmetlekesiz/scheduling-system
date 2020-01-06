        //typedef double ElementType;
        #define Infinity (6000000000L)

        #ifndef _BinHeap_H
		
		#define MaxTrees (12)   /* Stores 2^9 -1 items */
        #define Capacity (4095)

        struct BinNode;
        typedef struct BinNode *BinTree;
        struct Collection{
            int CurrentSize;
            BinTree TheTrees[ MaxTrees ];
        };
        typedef struct Collection *BinQueue;


        struct ElementType{
            int id;
            int e;
            int t; //arrival time
            int bhCounter;
            double pValue;
            int eProcess;
            int wt; //waiting time
        }typedef ElementType;

        BinQueue Initialize( void );
        void Destroy( BinQueue H );
        BinQueue MakeEmpty( BinQueue H );
        BinQueue Insert(ElementType Item, BinQueue H );
        ElementType DeleteMin( BinQueue H );
        BinQueue Merge( BinQueue H1, BinQueue H2 );
        ElementType FindMin( BinQueue H );
        int IsEmpty( BinQueue H );
        int IsFull( BinQueue H );
        BinTree printTree(BinTree p, BinTree *r, int i);
        BinTree increaseWaitingTimeBinomialTree(BinTree p, BinTree *r, int i, int increment);
        void increaseWaitingTime(BinQueue H, int i, int increment);
        BinTree treeTraversal(BinTree p, BinTree *r, int i, int eMax);
        double calculateC(ElementType *node, int eI, int eMax);
        double calculatePV(ElementType *node, int e, int t, int eMax);


#endif
/* END */