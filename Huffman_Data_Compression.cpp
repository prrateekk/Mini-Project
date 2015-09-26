#include <iostream>
#include <string.h>
using namespace std;

#define N 1000 // maximum characters
#define MAX 100 //maximum height of the tree

struct node{
    int freq;
    char key;
    struct node *left,*right;
};

struct node *heap[N];   //heap in the form of array of node addresses

int heapsize;
//will store keys and their frequencies
int a[N];
char keys[N];
//Log file for storing keys with their corresponding bit string
char key_log[N];
char bin_log[N][MAX];

//returns a node address with left and right children(may be NULL)
struct node *getnode(struct node *left,struct node *right){
    struct node *temp=new node();
    temp->left=left;
    temp->right=right;
    temp->key='\0';
    temp->freq=0;
    if (left!=NULL&&right!=NULL){
        temp->freq=(left->freq)+(right->freq);
    }
    return temp;
}

//swapping the nodes in the heap array during bubble up and bubble down operations
int swap_address(int a,int b){
    struct node* temp=heap[a];
    heap[a]=heap[b];
    heap[b]=temp;
    return 0;
}

//maintains the min-heap property moving downward from current root indexed i
int bubbledown(int i){
    int l=2*i+1;
    int r=l+1;
    int temp;
    if (l<heapsize&&r<heapsize){
        if (heap[l]->freq<heap[r]->freq)
            temp=l;
        else
            temp=r;
    }
    else if (l<heapsize)
        temp=l;
    else if (r<heapsize)
        temp=r;
    else
        return 0;

    if (heap[temp]->freq<heap[i]->freq){
        swap_address(temp,i);
        bubbledown(temp);
    }
    else return 0;
    return 0;
}

//returns the parent index of node indexed i. It may return an invalid negative result(
int parent(int i){
    if (i%2==0) return (i/2-1);
    else return (i/2);
}

//maintains the min-heap property moving upward from current root indexed i(rectified in bubble up.
int bubbleup(int i){
    if (i<0) return 0;  //negative parent index rectified
    int l=2*i+1;
    int r=l+1;
    int temp;
    if (l<heapsize&&r<heapsize){
        if (heap[l]->freq<heap[r]->freq)
            temp=l;
        else
            temp=r;
    }
    else if (l<heapsize)
        temp=l;
    else if (r<heapsize)
        temp=r;
    else
        return 0;

    if (heap[temp]->freq<heap[i]->freq){
        swap_address(temp,i);
        bubbleup(parent(i));
    }
    else return 0;
    return 0;
}

//removes and returns the priority node(the root) and also maintains the min-heap property by bubble down operation.
struct node *extract_node(){
    struct node *temp=heap[0];
    heap[0]=heap[heapsize-1];
    heap[heapsize-1]=NULL;
    heapsize--;
    bubbledown(0);
    return temp;
}

//inserts a node in the end at first and then maintains the min-heap property by bubble up operation.
int insert_node(struct node* temp){
    heap[heapsize]=temp;
    bubbleup(parent(heapsize));
    heapsize++;
    return 0;
}

//constructs the heap-array storing addresses of each element as independent heap tree.
int construct_heaparray(int a[],int n){
    heapsize=n;
    for (int i=0;i<heapsize;i++){
        heap[i]=getnode(NULL,NULL);
        heap[i]->freq=a[i];
        heap[i]->key=keys[i];
    }
    return 0;
}

//building min-heap
int build_min_heap(){
    for (int i=heapsize-1;i>=0;i--){
        bubbleup(i);
    }
    return 0;
}

//traversing the final heap generated to get the final bit-strings for each character and storing the record in the log-files
int c=0;
int traverse(struct node *temp,char bits[],int depth,char b){
    if (depth!=0)
    bits[depth-1]=b;
    bits[depth]='\0';
    if (temp->left==NULL&&temp->right==NULL){
        key_log[c]=temp->key;
        strcpy(bin_log[c],bits);
        c++;
        return 0;
    }
    traverse(temp->left,bits,depth+1,'0');
    traverse(temp->right,bits,depth+1,'1');
    return 0;
}

//main body for generating the heap-tree.
int construct_heap_tree(int n){
    construct_heaparray(a,n);
    build_min_heap();
    while (heapsize>1){
        struct node *min1=extract_node();
        struct node *min2=extract_node();
        struct node *temp=getnode(min1,min2);
        insert_node(temp);
    }
    char bits[MAX];
    traverse(heap[0],bits,0,'\0');
    return 0;
}

char s[100000];
int freq[N];
int main(){
    cin >> s;
    int i=0;
    while (s[i]!='\0'){
        freq[(int)s[i]]++;
        i++;
    }
    int c=0;
    for (int i=0;i<N;i++){
        if (freq[i]!=0){
            a[c]=freq[i];
            keys[c++]=char(i);
        }
    }
    construct_heap_tree(c);
    int bits1=0;
    for (int i=0;i<c;i++) bits1+=a[i];
    bits1*=8;
    int bits2=0;
    for (int i=0;i<c;i++){
        for (int j=0;j<c;j++){
            if (key_log[i]==keys[j]){
                bits2+=(strlen(bin_log[i])*a[j]);
                break;
            }
        }
    }
    cout << endl << "Initial memory consumption:" << bits1 << " bits." << endl;
    cout << "After Data Compression:" << bits2 << " bits." << endl;
    cout << ((float)(bits1-bits2)/8) << " bytes of memory saved";
    return 0;
}
