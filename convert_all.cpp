#include <bits/stdc++.h>
#define col 5
#define _s 30
using namespace std;

int init[20],fin[20],a=0,b=0;
string init_dfa[_s],fin_dfa[_s];
int _a = 0, _b = 0;

/* A binary tree Node has data, pointer to left child
   and a pointer to right child */
class Node
{
    public:
    char data;
    int position;
    string firstpos;
    string lastpos;
    bool nullable;

    Node (char data)
    {
        this->data = data;

    }

};

Node* tree[100];//list to save all the symbols
string followpos[30];//list to save the followpos
char dfa[30][3];//dfa table
string a_poses="null";//variable string for all a positions
string b_poses="null";//variable string for all b positions

// initiate the followpos list with null
int initfoll(){
    for(int i =0; i<30; i++){
        followpos[i]="null";
    }
    return 0;
}
// initiate the dfa list with 0
int initdfa(){
    for(int i =0; i<30; i++){
        for(int j=0; j<3;j++){
        dfa[i][j]='0';}
    }
    return 0;
}

/*
- give a position number for each operand (a, b, #)
- checking all the tree searching for operand + save all the position numbers for a in a_poses variable and
all the position numbers for b in b_poses variable to use for generating new sets in the DFA function
*/
int node_pos(){
    int i=0;
    int po=1;
    while (tree[i] != NULL){
        if((tree[i]->data==97)){
          tree[i] -> position=po;
          if(a_poses=="null"){
           a_poses=to_string(po);   
          }else{a_poses.append(to_string(po));}
          
           po++;
          }
          else if((tree[i]->data==98)){
          tree[i] -> position=po;
          if(b_poses=="null"){
           b_poses=to_string(po);   
          }else{b_poses.append(to_string(po));}
          
           po++;
          }
          else if(tree[i]->data==35){
            tree[i] -> position=po;
          }
        i++;

    }
  return 0;
}
/*
- function to check if the symbol is nullable or not
- all the operands not nullable
- '*' always nullable
- we have to check the children for each operators('.', '|')
- for '.' if the left child and the right child are nullable, then the '.' symbol is also nullable
- for '|', if the left child or the right child is nullable, then the '|' symbol is also nullable
*/
int node_null(){
    int i=0;
    while (tree[i] != NULL){
            if( tree[i]->data == '*'){
                    tree[i] -> nullable=true;}

            else if(tree[i]->data =='.'){
                
                if(tree[i-1]->data=='*'&&(tree[i-2]->data=='a'||tree[i-2]->data=='b')){
                        if(tree[i-1]->nullable==true && tree[i-3]->nullable==true)
                           {tree[i] -> nullable=true;}   
                    }
                    else if(tree[i-1]->data=='*'&&tree[i-2]->data=='|'){
                        if(tree[i-1]->nullable==true && tree[i-5]->nullable==true)
                           {tree[i] -> nullable=true;}  
                    }
                    else{
                        if(tree[i-1]->nullable==true && tree[i-2]->nullable==true)
                           {tree[i] -> nullable=true;}
                    }
                

            }

            else if(tree[i]->data =='|'){

            if(tree[i-1]->nullable==true || tree[i-2]->nullable==true)
                 tree[i] -> nullable=true;

            }

            i++;

    }
    return 0;
}

/*
- give each symbol firstpos number
- for the operand, the firstpos number is the same as the position number
- for the '*' symbol, the firstpos is his child's firstpos
- for the '|' symbol, the firstpos is the union of his children's firstpos
- for the '.' symbol, we must check the left child. If nullable, then the firstpos is the union of his children's firstpos
but if not, then '.' firstpos is his left child's firstpos
*/
int node_firstpos(){
    int i=0;
    while(tree[i] != NULL){
char x = tree[i]->data;
         switch(x){

            case 'a':   tree[i] -> firstpos= to_string(tree[i]->position) ;
                        break;
            case 'b':   tree[i] -> firstpos = to_string(tree[i]->position);
                        break;

            case '#':   tree[i] ->firstpos = to_string(tree[i]->position);
                        break;

            case '.':

                
                if(tree[i-1]->data=='*'&&(tree[i-2]->data=='a'||tree[i-2]->data=='b')){
                    if(tree[i-3]->nullable==true)
                        {tree[i]->firstpos= tree[i-1]->firstpos+tree[i-3]->firstpos;} 
                    else{
                        tree[i]->firstpos=tree[i-3]->firstpos;}
                }
                else if(tree[i-1]->data=='*'&&tree[i-2]->data=='|'){
                    if(tree[i-5]->nullable==true)
                        {tree[i]->firstpos= tree[i-1]->firstpos+tree[i-5]->firstpos;} 
                    else{
                        tree[i]->firstpos=tree[i-5]->firstpos;}
                }
                else {
                    if(tree[i-2]->nullable==true)
                        {tree[i]->firstpos= tree[i-1]->firstpos+tree[i-2]->firstpos;}
                    else{
                        tree[i]->firstpos=tree[i-2]->firstpos;}
                }
                    
                

                

                        break;
            case '|':

                 tree[i]->firstpos= tree[i-1]->firstpos +tree[i-2]->firstpos;
                        break;
            case '*':

                 tree[i]->firstpos= tree[i-1]->firstpos;;
                        break;
         }
        i++;
    }

return 0;
}
/*
- give each symbol lastpos number
- for the operand, the lastpos number is the same as the position number
- for the '*' symbol, the lastpos is his child's lastpos
- for the '|' symbol, the lastpos is the union of his children's lastpos
- for the '.' symbol, we must check the right child. If nullable, then the lastpos is the union of his children's lastpos
but if not, then '.' lastpos is his right child's lastpos
*/
int node_Laststpos(){
    int i=0;
    while(tree[i] != NULL){
char x = tree[i]->data;
         switch(x){

            case 'a':   tree[i] ->lastpos= to_string(tree[i]->position) ;
                        break;
            case 'b':   tree[i] ->lastpos = to_string(tree[i]->position);
                        break;

            case '#':   tree[i] ->lastpos = to_string(tree[i]->position);
                        break;

            case '.':

                if(tree[i-1]->nullable==true){
                    if(tree[i-1]->data=='*'&&(tree[i-2]->data=='a'||tree[i-2]->data=='b')){
                      tree[i]->lastpos= tree[i-1]->lastpos+tree[i-3]->lastpos;  
                    }
                    else if(tree[i-1]->data=='*'&&tree[i-2]->data=='|'){
                      tree[i]->lastpos= tree[i-1]->lastpos+tree[i-5]->lastpos;  
                    }
                    else{
                        tree[i]->lastpos= tree[i-1]->lastpos+tree[i-2]->lastpos;
                    }
                    
                }

                else{

                     tree[i]->lastpos=tree[i-1]->lastpos;
                }

                        break;
            case '|':

                    tree[i]->lastpos= tree[i-1]->lastpos+tree[i-2]->lastpos;
                        break;
            case '*':

                 tree[i]->lastpos= tree[i-1]->lastpos;
                        break;
         }
        i++;
    }

return 0;
}
/*
- using followpos list to fill the follow pos for each position
- by calculating just ('.','*') operators
- starting to search for the left and right for each one of them
- for the '.' symbol but the firstpos for the right child in each char in the lastpos for the left child
- for the '*' symbol but the lastpos for the left child in each char in the firstpos for the left child
*/
int node_followPos(){
    initfoll();//calling this method to initiate the followpos list with null
    int i=0;
    char x =tree[i]->data;
    string temp="null";// temp variable to save the set that will be saved in followpos[index]
    int l=0;
    char index='n';//variable with initial char 'n' that will hold the specific index in the followpos to save the temp on it
    while(tree[i] != NULL){

         switch(x){

            case '.':
                if(tree[i-1]->data=='*'&&(tree[i-2]->data=='a'||tree[i-2]->data=='b')){
                    temp = tree[i-1]->firstpos;
                    l =tree[i-3]->lastpos.length();
                    for( int j= 0;j<l;j++){
                    index= tree[i-3]->lastpos[j];
                    if(followpos[int(index- '0')]=="null"){
                        followpos[int(index- '0')]=temp;}
                        else{followpos[int(index- '0')].append(temp);}
                    }
                }
                else if(tree[i-1]->data=='*'&&tree[i-2]->data=='|'){
                    temp = tree[i-1]->firstpos;
                    l =tree[i-5]->lastpos.length();
                    for( int j= 0;j<l;j++){
                    index= tree[i-5]->lastpos[j];
                    if(followpos[int(index- '0')]=="null"){
                        followpos[int(index- '0')]=temp;}
                        else{followpos[int(index- '0')].append(temp);}
                    }
                }
                else{
                    temp = tree[i-1]->firstpos;
                    l =tree[i-2]->lastpos.length();
                    for( int j= 0;j<l;j++){
                    index= tree[i-2]->lastpos[j];
                    if(followpos[int(index- '0')]=="null"){
                        followpos[int(index- '0')]=temp;}
                        else{for(int n= 0;n<temp.length();n++){
                               if(followpos[int(index- '0')].find(temp[n])<followpos[int(index- '0')].length()){
                                }else{followpos[int(index- '0')].append(temp);}
                             }}
                    }
                }
                
                break;

            case '*':
                    temp = tree[i-1]->lastpos;
                    l =tree[i-1]->firstpos.length();
                    for(int j= 0;j<l;j++){
                    index=tree[i-1]->firstpos[j];
                    if(followpos[int(index- '0')]=="null"){
                        followpos[int(index- '0')]=temp;}
                        else{for(int n= 0;n<temp.length();n++){
                               if(followpos[int(index- '0')].find(temp[n])<followpos[int(index- '0')].length()){ 
                                }else{followpos[int(index- '0')].append(temp);}
                             } 
                            
                        }
                    }
                        break;
            case'a':
                break;
            case'b':
                break;
            case'|':
                break;
            case'#':
                break;

         }
                   i++;
                   if(tree[i] != NULL){
                       x=tree[i]->data;}

                   }
          return 0;
         }
/*
Receives a set and produces a new set
*/
string generate_set_a(string set){        
    int k=0;
    int i =0;
    string a_followes="null";//the followpos for 'a' symbol that found in this set
    string new_set="null";
            while(i<a_poses.length()){//by using a_poses variable we can check all the position for a in this set 
            if(set.find(a_poses[i])<set.length()){
                if(a_followes=="null"){
                    a_followes=a_poses[i];   
                }else{
                    a_followes=a_followes + a_poses[i];}
                
            }  
            i++;}
            i=0;
            
            while(i<a_followes.length()){//merge all the sets that found in the followpos for a_followes numbers to generate new set
                if(new_set=="null"){
                    new_set=followpos[int (a_followes[i]-'0')];   
                }else{
                    for(int n =0; n<followpos[int (a_followes[i]-'0')].length();n++){
                       if(new_set.find(followpos[int (a_followes[i]-'0')][n])<new_set.length()){
                       }else{
                            new_set=new_set + followpos[int (a_followes[i]-'0')][n]; } 
                            
                    }
                    
                    
                }
            i++;  
            }

        
        return new_set;
}
/*
Receives a set and produces a new set
*/
string generate_set_b(string set){        
    int k=0;
    int i =0;
    string b_followes="null";//the followpos for 'b' symbol that found in this set
    string new_set="null";
            while(i<b_poses.length()){//by using b_poses variable we can check all the position for b in this set 
            if(set.find(b_poses[i])<set.length()){
                if(b_followes=="null"){
                    b_followes=b_poses[i];   
                }else{b_followes=b_followes + b_poses[i];}
                
            }  
            i++;
            }
            
            i=0;
            while(i<b_followes.length()){//merge all the sets that found in the followpos for b_followes numbers to generate new set
                if(new_set=="null"){
                    new_set=followpos[int (b_followes[i]-'0')];   
                }else{
                    for(int n =0; n<followpos[int (b_followes[i]-'0')].length();n++){
                       if(new_set.find(followpos[int (b_followes[i]-'0')][n])<new_set.length()){
                       }else{
                            new_set=new_set + followpos[int (b_followes[i]-'0')][n];}  
                            
                    }
                    
                }
            i++;  
            }
        
        return new_set;
}
/*
build a dfa table by starting from the start state that is saved in the followpos[1]
*/
int DFA(string s){
        initdfa();//calling this method to initiate the dfa list with null

        string arr[30][2];//new list to save all the new states that will be calculated latter
        
        for(int m =0; m<30; m++){//initiate the arr list with null
        arr[m][0]="null";
        arr[m][1]="null";
        }
        string set=followpos[1];//start set
        arr[0][0]="states";
        arr[0][1]="sets";
        arr[1][0]='A';
        arr[1][1]=set;
        int i=1;
        
        int counter=1;//to save, how many sets do we have
        while(arr[i][1]!="null"){
            string new_set_a;//variable to save the new set that found from generate_set_a(arr[i][1])
            string new_set_b;//variable to save the new set that found from generate_set_b(arr[i][1])
            string ex_a="null";//variable to save the set that generated the new set that found from generate_set_a(arr[i][1])
            string ex_b="null";//variable to save the set that generated the new set that found from generate_set_b(arr[i][1])
            new_set_a= generate_set_a(arr[i][1]);
            new_set_b= generate_set_b(arr[i][1]);
                for(int a=0; a<30; a++)
                {if(new_set_a==arr[a][1]){
                  ex_a=arr[a][0]; 
                }if(new_set_b==arr[a][1]){
                  ex_b=arr[a][0];  
                }
                    
                }
                if(ex_a!="null"){
                    dfa[i][0]=arr[i][0][0];
                    dfa[i][1]=ex_a[0];
                }
                else {
                    counter++;
                    arr[counter][0]=arr[counter-1][0][0]+1;
                    arr[counter][1]=new_set_a;
                    dfa[i][0]=arr[i][0][0];
                    dfa[i][1]=arr[counter][0][0];
                    
                }
                if(ex_b!="null"){
                    dfa[i][0]=arr[i][0][0];
                    dfa[i][2]=ex_b[0];
                }
                else{
                    counter++;
                    arr[counter][0]=arr[counter-1][0][0]+1;
                    arr[counter][1]=new_set_b;
                    dfa[i][0]=arr[i][0][0];
                    dfa[i][2]=arr[counter][0][0];}

            i++;}

        dfa[1][0]='A';
        cout<<endl<<endl;
        for(int i=0;i<40;i++){//print dfa table
          cout<<"*";
          }cout<<endl<<endl;
        cout<<setw(30)<<"TRANSITION TABLE FOR DFA"<<endl<<endl;
        cout<<setw(10)<<"States"<<setw(10)<<"a"<<setw(10)<<"b"<<endl;
        for(int i=0;i<40;i++){
            cout<<"-";}
        cout<<endl;
        int n=1;
        while(dfa[n][2]!='0'){
            for(int j=0;j<3;j++){
            cout<<setw(10)<<dfa[n][j];
        }
        cout<<endl;
        n++;
       }
        

return 0;
          }
/*
function to bulid the symbol tree for RE with '#'
*/
void treebu(string s){
int l=s.length()-1;
for(int i=0;i<s.length();i++){
/*
The right child and the left child will be determined for each operator, 
In the "." symbol, not in all cases, the left child is i-2. There are subtrees in these children, 
so we have to calculate them in another way.
*/
    if(s[i]=='.'){
        
        tree[i]=new Node(s[i]);
        if(s[i-1]=='*'&&(s[i-2]=='a'||s[i-2]=='b')){
           tree[i-3]=new Node(s[i-3]); //left
           tree[i-1]=new Node(s[i-1]); //right
        }else if(s[i-1]=='*'&&s[i-2]=='|'){
           tree[i-5]=new Node(s[i-5]); //left
           tree[i-1]=new Node(s[i-1]); //right
        }else{
             tree[i-2]=new Node(s[i-2]); //left
             tree[i-1]=new Node(s[i-1]); //right 
        }
    }
    else if(s[i]=='|'){ 
        tree[i]=new Node(s[i]);
        tree[i-2]=new Node(s[i-2]); //left
        tree[i-1]=new Node(s[i-1]); //right
        }
    else if(s[i]=='*'){

        tree[i]=new Node(s[i]);
        tree[i-1]=new Node(s[i-1]); //left
    }
}

node_pos();// function to find the position for each symbol
node_null();// function to find the nullable for each symbol
node_firstpos();// function to find the firstpos for each symbol
node_Laststpos();// function to find the lastpos for each symbol
node_followPos();// function to find the followpos for '.' symbol, and '*' symbol

}


void initialise(int nfa_table[][col]){ // initiate the nfa table with -1
    for(int i=0;i<1000;i++){
        for(int j=0;j<col;j++){
            nfa_table[i][j]=-1;
        }
    }
}

void print_initial_final(){ // print the inintal state for both reg to nfa and nfa to dfa
    cout<<"initial state/s is/are :- ";
    for(int i=0;i<a;i++)
        cout<<init[i]<<" ";
        cout<<endl;
        cout<<"final state/s is/are :- ";
    for(int i=0;i<b;i++)
        cout<<fin[i]<<" ";
        cout<<endl;
}

void print_initial_final_dfa(){ // print the final state for both reg to nfa and nfa to dfa
    cout<<"initial state/s is/are :- ";
    for(int i=0;i<_a;i++)
        cout<<init_dfa[i]<<" ";
        cout<<endl;
        cout<<"final state/s is/are :- ";
    for(int i=0;i<_b;i++)
        cout<<fin_dfa[i]<<" ";
        cout<<endl<<endl;
    for(int i=0;i<60;i++)
            cout<<"-";
    cout<<endl<<endl;
}

void reduce_fin(int x){ // remove the final stale of symbol after map it with other symbol
    for(int i=x; i<b-1; i++)
        fin[i] = fin[i+1];
        b -= 1;
}
/*
 preprocessor function simplifies the expression where it add dot between letter to indicate concatenation
 operation in additon parentheses for priority. ex: (a|b)(ab|b) -> ((a|b).(a.b)|b))
*/

string preprocessor(string s){
    char x[5000];
    int l=s.length();
    int j=0;
    x[j]='('; /* starting by add open parenthese*/
    j += 1;
    for(int i=0;i<l;i++){
            x[j]=s[i]; /* insert char of the input string in x[j] */
            j += 1;
        if(((s[i]>=97&&s[i]<=122)||s[i]==35)&&((s[i+1]>=97&&s[i+1]<=122)||s[i+1]==35)){/*checking if s[i] and s[i+1] such as ab is a letter or #  if it letter then but a dot in x[j] */
            x[j]='.';
            j += 1; 
        }else if(s[i]==')'&&s[i+1]=='('){ /* checking if s[i] is ) and s[i+1] is ( such as )( if they add dot to x[i] */
            x[j]='.';
            j += 1;
        }else if(((s[i]>=97&&s[i]<=122)||s[i]==35)&&s[i+1]=='('){ /* checking if s[i] char is letter or # and s[i+1] is ( such as a( if they add dot to x[i] */
            x[j]='.';
            j += 1;
        }else if(s[i]==')'&&(s[i+1]>=97&&s[i+1]<=122||s[i+1]==35)){ /*checking if s[i] is ) and the s[i+1] is latter or # such as )a if they add dot to x[i] */
            x[j]='.';
            j += 1;
        }else if(s[i]=='*'&&(s[i+1]=='(' || ((s[i+1]>=97&&s[i+1]<=122)||s[i+1]==35))){ /* checking if s[i] is * and s[i+1] is ( or letter such as * ( or * a if they add dot to x[i] */
            x[j]='.';
            j += 1;
        }
    }
    x[j] = ')'; // now close the parenthese we opened earlier
    j += 1;
    string p;
    for(int i=0;i<j;i++) // reformat the string to be with dot and parentheses
       p += x[i];
    return p;
}

/*
    converts the preprocessed regular expression  into it's corresponding
    postfix notation
*/

string postfix(string s){
    int l=s.length();
    char a[5000];
    stack <char> ch;
    int j=0;
    for(int i=0;i<l;i++){
         char x = s[i]; // traverse over the input string char by char
         switch(x){
            case 'a':   a[j]='a';    /* if x is either a or b or # then just add it to the array a in a[j] */
                        j += 1;
                        break;
            case 'b':   a[j]='b';
                        j+=1;
                        break;

            case '#':   a[j]='#';
                        j+=1;
                        break;

            case '(':   ch.push('('); /* if x is either ( or ) then push it to the stack ch */
                        break;
            case ')':   while(!ch.empty()){
                            if(ch.top()=='('){
                                ch.pop();
                                break;
                            }else{
                                a[j]=ch.top();
                                    ch.pop();
                                    j += 1;
                            }
                        }
                        break;
            /** has a higher priority than the rest of the operations, so in case x is a lower priority operation , 
             we must pop * first before push the lower priority operations
             
            */
            case '.':   if(ch.empty()){ /* if x is . then check the stack ch if empty then just push . else check if the top of stack ch is ( if true then push . else if other operaters * then pop it and push the . */
                            ch.push('.');
                        }else {
                            char temp = ch.top();
                            if(temp=='(')
                                ch.push('.');
                            else if(temp=='*'){ /* in case the top is * then pop it and then add it to the array a  */
                                a[j]=ch.top();
                                ch.pop();
                                j += 1;
                                if(ch.top()== '.'){ // and after pop * the top is another . ,just add the . into array a
                                    a[j] = '.';
                                    j += 1;
                                }else{ // if top not another . then just push the current .
                                    ch.push('.');
                                }
                            }else if(temp=='.'){ // in case the top is another a then pop it and add it to array a and then push the current .
                                a[j]=ch.top();
                                ch.pop();
                                j += 1;
                                ch.push('.');
                        }else if(temp == '|'){ // or if top is | then just push the .
                               ch.push('.');
                        }
                    }
                        break;
            case '|':   if(ch.empty()){ // the same process for |
                            ch.push('|');
                        }else{
                            char temp = ch.top();
                            if(temp == '(')
                                ch.push('|');
                            else if(temp == '*'){
                                a[j] = ch.top();
                                ch.pop();
                                j += 1;
                                ch.push('|');
                            }else if(temp == '.'){
                                a[j] = ch.top();
                                j += 1;
                                ch.pop();
                                ch.push('|');
                            }
                        }
                            break;
            case '*':   if(ch.empty()){ //in case * then check if the stack empty just push it else check if the top is ( or . or | since these operations has lower priority then * so we can push the * to the stack
                            ch.push('*');
                        }else{
                            char temp = ch.top();
                            if(temp == '(' || temp == '.' || temp == '|' )
                                ch.push('*');
                            else{
                                a[j] = ch.top();
                                ch.pop();
                                j += 1;
                                ch.push('*');
                            }
                        }
                        break;
         }
    }
    string p; //
    for(int i=0;i<j;i++){
        p += a[i];
    }
    return p; // print the postfix form of the input string
}

/*
    this function convert's given postfix of reg expression
    into it's corresponding nfa
*/

int reg_nfa(string s,int nfa_table[][col]){ // receive the postfix strign and  table as matrix with five columns representing states, a, b, epsilon ,epsilon
    int l = s.length();
    int states = 1;
    int m,n,j,counter;
    for(int i=0;i<l;i++){
        char x = s[i];  // traverse over postfix string char by car
        switch(x){
            case 'a': nfa_table[states][0] = states;     // in case x is a or b then initiate the state(state=1) in nfa_table and add it to array init which carry initial states
                        init[a] = states;
                         a += 1;
                            states += 1;
                      nfa_table[states-1][1] = states; // link the initial state to the final state(state=2) of the input string and then add the final state to array which carry final states 
                        fin[b] = states;
                        b += 1;
                      nfa_table[states][0] = states; // initiate the new state(state=2) to the nfa_table
                            states +=1;
                      break;
            case 'b': nfa_table[states][0] = states; // the same process as a
                        init[a] = states;
                         a += 1;
                            states += 1;
                      nfa_table[states-1][2] = states;
                          fin[b] = states;
                          b += 1;
                      nfa_table[states][0] = states;
                            states +=1;
                      break;
            case '.': m = fin[b-2]; // in case x is . then we first link the two operand between the dot by link the first final state in fin array and the second initial state in array init
                      n = init[a-1]; //and the second initial state in array init
                      nfa_table[m][3]=n; // add the n to epsilon col in the m'th raw
                      reduce_fin(b-2);  //reduces final state , or delete the m since we link it with another state in the table
                      a -= 1;           //reduces initial states, or delete the n since we link it with another state in the table
                      break;
            case '|': for(j=a-1,counter=0;counter<2;counter++){ // in case x is | then we initiate an initial state that connects the initial states we initiated earlier
                        m = init[j-counter];
                        nfa_table[states][3+counter]=m;// with epsilon as input for each of them
                      }
                      a=a-2; // after we connected the initial states now it's time to delete them from the init array
                      init[a]=states; // and add the new initial state to the array  
                      a += 1;
                      nfa_table[states][0] = states; // add the new state to the table
                      states += 1;
                      //We now connect the final states that were previously linked
                      // to the initial states with the new states to become the current final states
                      for(j=b-1,counter=0;counter<2;counter++){ 
                        m = fin[j-counter];
                        nfa_table[m][3]=states;
                      }
                      b=b-2; // after we connected the final states now it's time to delete them from the fin array
                      fin[b]=states; // and add the new final satate to array
                      b += 1;
                      nfa_table[states][0] = states; // add the new state to the table
                      states += 1;
                      break;
            case '*': m = init[a-1]; // in case x is * then first we take the current initial state and connecte with last state added which the current final
                      nfa_table[states][3] = m; // initial state connected to the final with epsilon
                      nfa_table[states][0] = states; // add the current final state to the table
                      init[a-1] = states; //after we connected the initial states now it's time to delete them from the init array

                      states += 1;
                      n = fin[b-1]; // now we take the current final and connecting it in reverse dirction to current initial state
                      nfa_table[n][3]=m; // with epsilon 
                      nfa_table[n][4]=states; // then the current final state is connect to last state which is the final now
                      nfa_table[states-1][4]=states;
                      fin[b-1]=states;
                      nfa_table[states][0]=states;
                      states += 1;
                        break;
        }
    }
  return states;
}

void print_nfa_table(int nfa_table[][col],int states){
    cout<<endl;
    for(int i=0;i<60;i++)
        cout<<"*";
        cout<<endl<<endl;
    cout<<setw(43)<<"TRANSITION TABLE FOR NFA"<<endl<<endl; // setw function puts a specific space before printing 
    cout<<setw(10)<<"States"<<setw(10)<<"a"<<setw(10)<<"b"<<setw(10)<<"e"<<setw(10)<<"e"<<endl; // this for table format
    for(int i=0;i<60;i++)
        cout<<"-";
        cout<<endl;
    for(int i=1;i<states;i++){
        for(int j=0;j<col;j++){
            if(nfa_table[i][j]==-1)
                cout<<setw(10)<<"--";
            else
                cout<<setw(10)<<nfa_table[i][j]; // print the states in the table
        }
        cout<<endl;
    }

    cout<<endl;
    for(int i=0;i<60;i++)
        cout<<"*";
    cout<<endl;
    print_initial_final();
}

void print_dfa_table(string dfa_tab[][3],int state){ // the same as print nfa_table
    cout<<endl<<endl;
    for(int i=0;i<60;i++)
        cout<<"*";
    cout<<endl<<endl;
    cout<<setw(43)<<"TRANSITION TABLE FOR DFA"<<endl<<endl; 
    cout<<setw(10)<<"States"<<setw(10)<<"a"<<setw(10)<<"b"<<endl;
    for(int i=0;i<60;i++)
        cout<<"-";
    cout<<endl;
    for(int i=0;i<state;i++){
        for(int j=0;j<3;j++){
            cout<<setw(10)<<dfa_tab[i][j];
        }
        cout<<endl;
    }
    cout<<endl;
    for(int i=0;i<60;i++)
        cout<<"*";
    cout<<endl;
    print_initial_final_dfa();
}

// this method traverse over nfa table to find E-closure of the state
vector <int> eclosure(int nfa_table[][col], int x){ // receive the state we want to find it's E-closure and the nfa_table
    stack <int> s;
    map <int, int> m;
    vector <int> v;
    int y;

    s.push(x); 
    m[x] = 1;

    while(!s.empty()){ //if the stack not empty then pop the state and save it to y 
        y = s.top();
            s.pop();
        if(nfa_table[y][3] == -1) // check if y haven't transition with epsilon in 3rd col
            continue; // true so ignore it
        else{
            s.push(nfa_table[y][3]); // else if y have transition to another state with epsilon 
            m[nfa_table[y][3]] = 1; // then save that state in map and marked with 1 to indicate that state is visted

            if(nfa_table[y][4] == -1) // check the 4th col of the y if it's empty then countinue
                continue;
            else{
                s.push(nfa_table[y][4]); // if not then push it to the stack 
                m[nfa_table[y][4]] == -1; // and marked as visted 
            }
        }
    }

    map <int, int> ::iterator itr;
    itr = m.begin();

    while(itr != m.end()){ // iiterat over the m and save the E-closure of the x 
        v.push_back(itr->first);
        itr++;
    }
    return v; // retrun a vector of the E-closure of the x
}

long long int map_it(vector <int> v, int y){
    long long int x=0,m=1;
    while(y--){
        m *=10;
    }

    vector<int> ::iterator it=v.begin();
    while(it != v.end()){
          x += *it*m;
          m /=10;
        it += 1;
    }
  return x/10;
}

/*
    The function state_name() is giving a new name to all new dfa state.
*/
string state_name(int i){
    char s = 'q';
    string p ;
    p += s;
    if(i==0){
            p += '0';
        return p;
    }
   int a[100];
   int j=0;
    while(i>0){
        int x = i%10;
        a[j]=x;
        j += 1;
        i = i/10;
    }
    for(int i=j-1;i>=0;i--){
            int x = a[i];
        switch(x){
            case 0: p += '0';
                    break;
            case 1: p += '1';
                    break;
            case 2: p += '2';
                    break;
            case 3: p += '3';
                    break;
            case 4: p += '4';
                    break;
            case 5: p += '5';
                    break;
            case 6: p += '6';
                    break;
            case 7: p += '7';
                    break;
            case 8: p += '8';
                    break;
            case 9: p += '9';
                    break;

        }
    }
    return p;
}

void check_init(vector <int> v, string s){ // find the initail state for DFA
    for(int i=0;i<v.size();i++){
        if(v[i] == init[0]){ // c
            init_dfa[_a] = s;
            _a += 1;
        }
    }
}

void check_fin(vector <int> v, string s){ // find final state for DFA
    for(int i=0;i<v.size();i++){
        if(v[i] == fin[0]){
            fin_dfa[_b] = s;
            _b += 1;
        }
    }
}

int nfa_dfa(int nfa_table[][col],int states,string dfa_tab[][3]){
    bool flag[states];
    memset(flag,true,sizeof(flag)); //to make sure all states E-closure found

    int state = 0,j = 0;
    map <vector<int>,string> map_state;
    vector <int> v,v1,v2,v3,v4;

    v = eclosure(nfa_table,init[0]); // find the  e-closure for the initial state
    flag[init[a]] = false; // mark the initial as visted

    map_state[v] = state_name(j++); // add linke initial e-closure with name
    check_init(v,map_state[v]);  //to check whether current state is initial or not
    check_fin(v,map_state[v]); //to check whether current state is final or not

   /* cout<<endl<<map_state[v]<<"represents :- ";
                for(int i=0;i<v.size();i++)
                    cout<<v[i]<<" ";
                cout<<endl; */

    stack < vector<int> > st;
    st.push(v); // push the e-closure to stack to find the transition of eachs state in  e-closure
    // transition of e-closure to over input symbol a
    int counter = 0;
    while(true){

       while(!st.empty()){
        vector <int> v ;
        v  = st.top();
             st.pop();
        counter += 1;
        dfa_tab[state][0] = map_state[v]; // initiate state in dfa table 

        for(int i=0;i<v.size();i++){ //traverse over e-closure to find the transiton  over a and b 
            flag[v[i]] = false; // mark v[i] as visted which is the first state in the e-closure
            int temp = nfa_table[v[i]][1];      //save the state that a comes as input 
            int temp1 = nfa_table[v[i]][2];     //save the state that b comes as input 
            if(temp>=0) // check if not empty 
                v1.push_back(temp); // push that state to v1
            if(temp1>=0)
                v3.push_back(temp1);
        }

        map <int,int> map_temp,map_temp1;  //using map to remove duplicate state
        map <int,int> ::iterator it;

        for(int i=0;i<v1.size();i++){ //traverse ocer each state that receive a as input
            v2 = eclosure(nfa_table,v1[i]); //find the e-closure of each state that receive a as input
            for(int j=0;j<v2.size();j++){ // add it to map to prevent duplicate states and marked as visted
                map_temp[v2[j]] = 1; 
            }
            v2.clear();
        }

        for(int i=0;i<v3.size();i++){ // the same process with b
            v4 = eclosure(nfa_table,v3[i]);
            for(int j=0;j<v4.size();j++){
                map_temp1[v4[j]] = 1;
            }
            v4.clear();
        }


        for(it = map_temp.begin(); it != map_temp.end(); it++){ // push the states receives a as input to v2
            v2.push_back(it->first);
            flag[it->first] = false;
        }

        for(it = map_temp1.begin(); it != map_temp1.end(); it++){
            v4.push_back(it->first);
            flag[it->first] = false;
        }

        if(v2.empty()){ // if there is now state in v2 then a col is has no transition for a
            dfa_tab[state][1] = "--";
        } else {
            string t = map_state[v2]; //else take the name of the state thar receive as input
            char flag1 = t[0];
            if( flag1 == 'q'){
                dfa_tab[state][1] = map_state[v2];   //checking the state has already been mapped to other state which send a 
            } else {
                dfa_tab[state][1] = state_name(j++); // if not then mapp such q0 -> q1
                map_state[v2] = dfa_tab[state][1]; // add the state to map_state to prevent the dupliction
                check_init(v2,map_state[v2]);
                check_fin(v2,map_state[v2]);
           /*-     cout<<endl<<map_state[v2]<<" represents :- ";
                for(int i=0;i<v2.size();i++)
                    cout<<v2[i]<<" ";
                cout<<endl; */
                st.push(v2);      //not mapped state will be pushed into stack
            }
        }

        if(v4.empty()){ // the same process for b
            dfa_tab[state][2] = "--";
        } else {
            string t = map_state[v4];
            char flag1 = t[0];
            if( flag1 == 'q'){
                dfa_tab[state][2] = map_state[v4];
            } else {
                dfa_tab[state][2] = state_name(j++);
                map_state[v4] = dfa_tab[state][2];
                check_init(v4,map_state[v4]);
                check_fin(v4,map_state[v4]);
               /* cout<<endl<<map_state[v4]<<" represents :- ";
                for(int i=0;i<v4.size();i++)
                    cout<<v4[i]<<" ";
                cout<<endl; */
                st.push(v4);
            }
        }
        v1.clear();
        v2.clear();
        v3.clear();
        v4.clear();
       state += 1;
    }

        int k = 1;
        for(k=1;k<states;k++){
            if(flag[k]){
                v = eclosure(nfa_table,k);
                map_state[v] = state_name(j++);
                check_init(v,map_state[v]);
                check_fin(v,map_state[v]);
                cout<<endl<<map_state[v]<<" represents :- ";
                for(int i=0;i<v.size();i++)
                    cout<<v[i]<<" ";
                cout<<endl;
                st.push(v);
                break;
            }
        }

        if(k == states)
                break;

    }

    print_dfa_table(dfa_tab,state);     //function to print dfa table

    return state;


}



int main(){
    cout<<"Welcome :)  "<<endl;
    cout<<"You have two choices :-"<<endl;
    cout<<"from regular expression to NFA to DFA enter 1"<<endl;
    cout<<"from regular expression to DFA enter 2"<<endl;
    cout<<"to exit enter any key"<<endl;
    int x;//to save user decision
    cin>>x;
    

    
    if(x==1){
        
        cout<<"Enter a regular expression :-  "<<endl<<"e.g. (a|b)*abb"<<endl;
        string s;
        cin>>s;

        s=preprocessor(s);
        cout<<"after preprocessed "<<s;
        cout<<endl;

        s=postfix(s);
        cout<<"postfix "<<s<<endl;

        int nfa_table[1000][col];
        initialise(nfa_table);
        int states=0;
            
        states = reg_nfa(s,nfa_table);//convert a given postfix of reg expression into it's corresponding nfa
        print_nfa_table(nfa_table,states);

        string dfa_tab[states][3];
        int dfa_state = 0;
        dfa_state = nfa_dfa(nfa_table,states,dfa_tab);//convert a nfa into dfa
    }
    else if(x==2){
        
        cout<<"Enter a regular expression :-  "<<endl<<"e.g. (a|b)*abb"<<endl;
        string s;
        cin>>s;

        s=preprocessor(s);
        cout<<"after preprocessed "<<s;
        cout<<endl;

        s=s+'#';


        s=preprocessor(s);
        cout<<"after preprocessed "<<s;
        cout<<endl;

        s=postfix(s);
        cout<<"postfix "<<s<<endl;
        treebu(s);// to build the tree with all the positions
        DFA(s);// to find all the possible states from the start state
        }
    else{cout<<"It was nice to see you :)";  
            }
 return 0;
}