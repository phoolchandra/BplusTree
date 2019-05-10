#include <bits/stdc++.h>
using namespace std;

int dataorder;
int indexorder;

typedef struct node{
	void **pointers;
	int *keys;
	struct node *parent;
	bool is_leaf;
	int num_keys;
	struct node *next;
}node;

typedef struct record{
	int value;
}record;

node *find_leaf(node *root,int key);
node *make_node();
node * make_leaf(void);
node *start_new_node(node *root,int key);
node *insert_into_leaf(node* leaf,int key);
node * insert_into_new_root(node * left, int key, node * right);
int get_left_index(node * parent, node * left);
node * insert_into_node(node * root, node * n, int left_index, int key, node * right);
node * insert_into_parent(node * root, node * left, int key, node * right);
int cut(int length);
node *insert_into_leaf_after_splitting(node *root,node *leaf,int key);
node *insert(node *root,int key);

void print_like_sir(node *root);

node *find_leaf(node *root, int key){
	node *c = root;
	int i;

	if(root==NULL)                                   
		return NULL;

	while(!c->is_leaf)
	{
		i=0;
		while( i < c->num_keys){
			if(key >= c->keys[i]) i++;
			else break;
		}

		c = (node *)c->pointers[i];
	}

	return c;
   
}


node *make_node(void)
{
	node *new_node;
	new_node = (node *)malloc(sizeof(node));
	new_node->keys = (int *)malloc((dataorder)*sizeof(int));
	new_node->pointers = (void **)malloc((dataorder +1)*sizeof(void *));
	new_node->is_leaf = false;
	new_node->num_keys = 0;
	new_node->parent = NULL;
	new_node->next = NULL;
	return new_node;
}

node *make_leaf_node(void)
{
	node *new_node;
	new_node = (node *)malloc(sizeof(node));
	new_node->keys = (int *)malloc((dataorder)*sizeof(int));
	new_node->pointers = (void **)malloc((dataorder +1)*sizeof(void *));
	new_node->is_leaf = false;
	new_node->num_keys = 0;
	new_node->parent = NULL;
	new_node->next = NULL;
	return new_node;
}

node *make_index_node(void)
{
	node *new_node;
	new_node = (node *)malloc(sizeof(node));
	new_node->keys = (int *)malloc((indexorder)*sizeof(int));
	new_node->pointers = (void **)malloc((indexorder +1)*sizeof(void *));
	new_node->is_leaf = false;
	new_node->num_keys = 0;
	new_node->parent = NULL;
	new_node->next = NULL;
	return new_node;
}


node * make_leaf()
{
	node *leaf = make_node();
	leaf->is_leaf = true;
	return leaf;
}


node *start_new_tree(node *root,int key){

	
	root = make_leaf();
	root->keys[0] = key;
	for(int i=0;i<=dataorder;i++)
	root->pointers[i] = NULL;
	root->parent = NULL;
	root->num_keys++;
	return root;
}

node *insert_into_leaf(node* leaf,int key){
	int i=0, insertion_point=0;

	while(insertion_point < leaf->num_keys && leaf->keys[insertion_point] < key)
		insertion_point++;

	for(i=leaf->num_keys;i>insertion_point;i--)
	{
		leaf->keys[i] = leaf->keys[i-1];
		leaf->pointers[i] = leaf->pointers[i-1];

	}

	leaf->keys[insertion_point] = key;
	leaf->pointers[insertion_point] = NULL;
	leaf->num_keys++;
    cout<<leaf->keys[insertion_point]<<endl;
	return leaf;
}


node * insert_into_new_root(node * left, int key, node * right)
{

    node * root = make_node();
    root->keys[0] = key;
    root->pointers[0] = left;
    root->pointers[1] = right;
    root->num_keys++;
    root->parent = NULL;
    left->parent = root;
    right->parent = root;
    return root;
}


int get_left_index(node * parent, node * left)
{

    int left_index = 0;
    while (left_index <= parent->num_keys &&
            parent->pointers[left_index] != left)
        left_index++;
    return left_index;
}

node * insert_into_node(node * root, node * n, int left_index, int key, node * right)
{
    int i;

    for (i = n->num_keys; i > left_index; i--)
    {
        n->pointers[i + 1] = n->pointers[i];
        n->keys[i] = n->keys[i - 1];
    }
    n->pointers[left_index + 1] = right;
    n->keys[left_index] = key;
    n->num_keys++;
    return root;
}

node * insert_into_node_after_splitting(node * root, node * old_node, int left_index, int key, node * right)
{

    int i, j, split, k_prime;
    node * new_node, * child;
    int * temp_keys;
    node ** temp_pointers;

   

    temp_pointers = (node **)malloc((indexorder+2) * sizeof(node *));
    temp_keys = (int *)malloc((indexorder+1) * sizeof(int));
    
    for (i = 0, j = 0; i < old_node->num_keys + 1; i++, j++)
    {
        if (j == left_index + 1) j++;
        temp_pointers[j] = (node *)old_node->pointers[i];
    }

    for (i = 0, j = 0; i < old_node->num_keys; i++, j++)
    {
        if (j == left_index) j++;
        temp_keys[j] = old_node->keys[i];
    }

    temp_pointers[left_index + 1] = right;
    temp_keys[left_index] = key;

  
    split = cut(indexorder)+1;
    new_node = make_index_node();
    old_node->num_keys = 0;


    for (i = 0; i < split-1; i++)
    {
        old_node->pointers[i] = temp_pointers[i];
        old_node->keys[i] = temp_keys[i];
        old_node->num_keys++;
    }

    old_node->pointers[i] = temp_pointers[i];
    k_prime =temp_keys[split-1];

    for (++i, j = 0; i < indexorder+1; i++, j++)
    {
        new_node->pointers[j] = temp_pointers[i];
        new_node->keys[j] = temp_keys[i];
        new_node->num_keys++;
    }
    new_node->pointers[j] = temp_pointers[i];
    free(temp_pointers);
    free(temp_keys);

    new_node->parent = old_node->parent;
    for (i = 0; i <= new_node->num_keys; i++)
    {
        child = (node *)new_node->pointers[i];
        child->parent = new_node;
    }

    

    return insert_into_parent(root, old_node, k_prime, new_node);
}

int cut(int length)
{
    if (length % 2 == 0)
        return length/2;
    else
        return length/2 + 1;
}

void cut1(int length,int &split1, int &split2, int &split3)
{
    if(length%3==0)
    {
    	split1 = length/3;
    	split2 = length/3;
    	split3 = length/3;

    }
    else if(length%3==1)
    {
    	split1 = (length/3)+1;
    	split2 = length/3;
    	split3 = length/3;
    }
    else
    {
    	split1 = (length/3)+1;
    	split2 = (length/3)+1;
    	split3 = length/3;
    }

    return ;
}


node * insert_into_parent(node * root, node * left, int key, node * right)
{

    int left_index;
    node * parent;

    parent = left->parent;

   

    if (parent == NULL)
    {
        return insert_into_new_root(left, key, right);
    }

    left_index = get_left_index(parent, left);

   

    if (parent->num_keys < indexorder)
        return insert_into_node(root, parent, left_index, key, right);

   

    return insert_into_node_after_splitting(root, parent, left_index, key, right);
}


node *insert_into_leaf_after_splitting(node *root,node *leaf,int key){
	node * new_leaf;
	node *new_leaf1;
	node *new_leaf2;
	int *temp_keys;
	void ** temp_pointers;
	int insertion_index, split, new_key, i, j;


	new_leaf1 = make_leaf();
	new_leaf2 = make_leaf();

	temp_keys = (int *)malloc((dataorder +1)*sizeof(void *));

	temp_pointers = (void **)malloc((dataorder+2)*sizeof(void *));

	insertion_index = 0;
	while(insertion_index < dataorder && leaf->keys[insertion_index] < key)
		insertion_index++;


	for(i=0,j=0;i < leaf->num_keys;i++,j++)
	{
		if(j==insertion_index)j++;
		temp_keys[j] = leaf->keys[i];
		temp_pointers[j] = NULL;
	}

	temp_keys[insertion_index] = key;
	temp_pointers[insertion_index] = NULL;

	leaf->num_keys = 0;
	new_leaf1->num_keys = 0;
	new_leaf2->num_keys = 0;
    
    int split1=0,split2=0,split3=0;
    cut1(dataorder+1,split1,split2,split3);
    cout<<split1<<" "<<split2<<" "<<split3<<endl;

	for(i=0;i<split1;i++)
	{
		leaf->pointers[i] = NULL;
		leaf->keys[i] = temp_keys[i];
		leaf->num_keys++;
	}

	for(i=split1,j=0;i<split1+split2;i++,j++)
	{
		new_leaf1->pointers[j] = NULL;
		new_leaf1->keys[j] = temp_keys[i];
		new_leaf1->num_keys++;
	}

	for(i=split2+split1,j=0;i<=dataorder;i++,j++)
	{
		new_leaf2->pointers[j] = NULL;
		new_leaf2->keys[j] = temp_keys[i];
		new_leaf2->num_keys++;
	}

	free(temp_pointers);
	free(temp_keys);


   // cout<<"split over"<<endl;
	
	new_leaf2->next = leaf->next;
	new_leaf1->next = new_leaf2;
	leaf->next = new_leaf1;
   
	for(i=leaf->num_keys ;i <dataorder ;i++)
		leaf->pointers[i] = NULL;
   
	for(i=new_leaf1->num_keys;i<dataorder;i++)
		new_leaf1->pointers[i] = NULL;
    // cout<<"split3 over"<<endl;
	for(i=new_leaf2->num_keys;i<dataorder;i++)
		new_leaf2->pointers[i] = NULL;
  
	new_leaf1->parent = leaf->parent;
   
    int n1=0,n2=0;
    n1 = leaf->num_keys;
    n2 = new_leaf1->num_keys;
    // cout<<n1<<" "<<n2<<endl;
    int new_key1,new_key2;
	new_key1 = leaf->keys[n1-1]+1;
	new_key2 = new_leaf1->keys[n2-1]+1;
	// cout<<"keys "<<new_key1<<" "<<new_key2<<endl;
    // cout<<"split6 over"<<endl;
	root = insert_into_parent(root,leaf,new_key1,new_leaf1);
	new_leaf2->parent = new_leaf1->parent;
	// cout<<"split7 over"<<endl;
	// print_like_sir(root);
	return insert_into_parent(root,new_leaf1,new_key2,new_leaf2);

}

node *insert(node *root,int key)
{
	record *record_pointer = NULL;
	node *leaf = NULL;
    node *fnd_leaf=NULL;
    
    if(root ==NULL)
    	return start_new_tree(root, key);

    leaf = find_leaf(root,key);

    if(leaf->num_keys < dataorder)
    {
        insert_into_leaf(leaf,key);
        return root;
    }

    return insert_into_leaf_after_splitting(root, leaf, key);




}


void find_and_print(node * root, int key)
{
    
    int i = 0;
    node * leaf = NULL;

    leaf = find_leaf(root, key);

    for (i = 0; i < leaf->num_keys; i++)
        {
        	if (leaf->keys[i] == key)
        	{ 
                cout<<"present"<<endl;
        		return ;
        	}
        }

        cout<<"given key is not present"<<endl;
    
}

int find_level(node *root, int key){
	node *c = root;
	int i;

	if(root==NULL)                                   
		return 0;
	int lev=0;

	while(!c->is_leaf)
	{
		i=0;
		while( i < c->num_keys){
			if(key >= c->keys[i]) i++;
			else break;
		}

		c = (node *)c->pointers[i];
		lev++;
	}

	return lev;
   
}

int store_level(node *root, int key){
	node *c = root;
	int i;

	if(root==NULL)                                   
		return 0;
	int lev=0;

	while(!c->is_leaf)
	{
		i=0;
		while( i < c->num_keys){
			if(key >= c->keys[i]) i++;
			else break;
		}

		c = (node *)c->pointers[i];
		lev++;
	}

	return lev;
   
}

void store(node *root, vector<vector<int> > &v,int lev)
{
    int i=0,j=0;

    node *c;
    c = root;
    if(root==NULL)
    	{
    		// cout<<"empty tree"<<endl;

    		return ;
    	}

    while(i<c->num_keys)
    {
    	// cout<<c->keys[i]<<" ";
    	v[lev].push_back(c->keys[i]);
    	i++;
    }


    v[lev].push_back(-1);

    i=0;
    
    while(i<=c->num_keys)
    {
    	store((node *)c->pointers[i],v,lev+1);
    	i++;
    }

}

void print_tree(node *root)
{
	node *c =root;
	if(root==NULL)
	{
		// cout<<"tree is empty"<<endl;
		return ;
	}
	int lev= find_level(root, INT_MAX);
	
	vector<vector<int> > v(lev+1);
	store(root, v, 0);
    cout<<"*********** level :: nodes  ***********"<<endl;
	for(int i=0;i<=lev;i++)
	{
		cout<<"level "<<i<<"  ";
		for(int j=0;j<v[i].size();j++)
			{
				if(v[i][j]==-1)
					cout<<"  |   ";
				else cout<<v[i][j]<<" ";
			}

		cout<<endl;
	}
}

void print_like_sir(node *root)
{
	node *c =root;
	if(root==NULL)
	{
		// cout<<"tree is empty"<<endl;
		return ;
	}
	int lev= find_level(root, INT_MAX);
	
	vector<vector<int> > v(lev+1);
	store(root, v, 0);
    cout<<"***** node ID :: parent ID:: values ***********"<<endl;
    cout<<endl;
    int node_num=1;
	
    
    vector<int> pa;
    int arr[100];
    int y=0;
    int ng=0;
    for(int r=0;r<v[lev-1].size();r++)
    {
       if(v[lev-1][r]==-1)
       {
        	arr[r]=y;
        	pa.push_back(y);
       	    y++;
       	    ng++;
       }
       else
       {
       	 	pa.push_back(y);
       	 	arr[r]=y;
       }
    }
    
    

    int nd=0;
    int x=0;
    if(ng==0)
    {
    	for(int j=0;j<v[lev].size()-1;j++)
		{
			// cout<<pa.size()<<arr[x]<<endl;
			cout<<"Data Node "<<x<<" Parent: Index node "<<"NULL"<<" ";
			for(;j<v[lev].size()-1;j++)
			{
				 if(v[lev][j]==-1)
			    	break;
				cout<<v[lev][j]<<" ";

			}
			// cout<<endl;
			cout<<"\n"<<endl;
				nd++;
				x++;
				// cout<<pa[4]<<endl;
		}
    }
	else{	for(int j=0;j<v[lev].size()-1;j++)
		{
			// cout<<pa.size()<<arr[x]<<endl;
			cout<<"Data Node "<<x<<" Parent: Index node "<<arr[x]<<" ";
			for(;j<v[lev].size()-1;j++)
			{
				 if(v[lev][j]==-1)
			    	break;
				cout<<v[lev][j]<<" ";

			}
			// cout<<endl;
			cout<<"\n"<<endl;
				nd++;
				x++;
				// cout<<pa[4]<<endl;
		}
	}
	// }
	

}


void datanodes(node *root)
{

	node * leaf = NULL;

    leaf = find_leaf(root, INT_MIN);

    while(leaf!= NULL)
    {
    	int i=0;
    	while(i < leaf->num_keys)
    	{
    		cout<<leaf->keys[i]<<" ";
    		i++;
    	}
    	leaf = leaf->next;
    }

}

void rangesearch(node *root, int key1,int key2)
{
	node * leaf = NULL;

    leaf = find_leaf(root, key1);
    bool flag=false;

    while(leaf!= NULL)
    {
    	int i=0;
    	while(i < leaf->num_keys && leaf->keys[i]<=key2)
    	{
    		if(leaf->keys[i]>=key1)
    	    	flag = true;

    		if(flag)
    		cout<<leaf->keys[i]<<" ";


    		i++;
    	}
    	leaf = leaf->next;
    }
}


int main(){

	int choice;
	int key,key1,key2;
	node *root=NULL;

	while(true){
	
       cout<<"enter 1 to create a new Btree"<<endl;
       cout<<"enter 2 to insert a key"<<endl;
       cout<<"enter 3 to search the key"<<endl;
       cout<<"enter 4 to print the tree"<<endl;
       cout<<"enter any key to terminate the programm"<<endl;
       cin>>choice;
       switch(choice){
        
        case 1: cout<<"enter the order of dataorder and indexorder\n"<<endl;
                cin>>indexorder>>dataorder; break;
       
        case 2:cout<<"enter the key\n"; cin>>key; root = insert(root,key); break;
        case 3: cout<<"enter a key \n"<<endl; cin>>key;find_and_print(root,key); break;
        case 7: cout << "print tree\n" <<endl; print_tree(root); break;
        case 5: datanodes(root); break;
        case 6: cout<<"enter key1 and key2 "<<endl;cin>>key1>>key2;rangesearch(root,key1,key2); break;
        case 4: print_tree(root); break;
        default : return 0;
           
       }
	}

	return 0;
}