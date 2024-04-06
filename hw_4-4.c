#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STACK_SIZE 100

typedef int element;
typedef struct{
element data[MAX_STACK_SIZE];
int top;
}StackType;

void init_stack(StackType *s)
{
    s->top=-1;
}

int is_empty(StackType *s)
{
    return (s->top==-1);
}

int is_full(StackType *s)
{
    return (s->top==(MAX_STACK_SIZE - 1));
}

void push(StackType *s,element item)
{
    if(is_full(s)){
        fprintf(stderr,"���� ��ȭ ����\n");
        return;
    }
    else s->data[++(s->top)]=item;
}

element pop(StackType *s)
{
   if(is_empty(s)){
        fprintf(stderr,"���� ���� ����\n");
        exit(1);
    }
    else return s->data[(s->top)--];
}

element peek(StackType *s)
{
   if(is_empty(s)){
        fprintf(stderr,"���� ���� ����\n");
        exit(1);
    }
    else return s->data[(s->top)];
}

int eval(char exp[])    //(���ÿ��� ���ڸ� ���� )
{
    int op1,op2,value,i=0;
    int len=strlen(exp);
    char ch;
    StackType s;

    init_stack(&s);   //���� �ʱ�ȭ
    for(i=0;i<len;i++){
        ch=exp[i];           //�Է��� ����
        if(isdigit(ch)){   //�Է��� ������ �����ϰ��
            value=ch-'0';  //�ƽ�Ű�ڵ� 0�� �� �������� ���������� �ٲ�
            push(&s,value);//���ÿ� ���ڸ� ����
        }
        else{
            op2=pop(&s);  //�������� ���
            op1=pop(&s);   //op2,1�� ���� ������ ���� 2���� ����
            switch(ch){
            case '+':push(&s,op1+op2);break;  //����
            case '-':push(&s,op1-op2);break;
            case '*':push(&s,op1*op2);break;
            case '/':push(&s,op1/op2);break;
            }
        }
    }
    return pop(&s); //���������� ����==���
}

int check_matching(const char *in)
{
    StackType s;
    char ch,open_ch;    //ch�Է¹��� ���� �ϳ��� Ȯ���ϱ� ���� ����, open ch�ݴ� ��ȣ�� �������� ���ÿ��� �������� ���� ��ȣ
    int i,n=strlen(in);
    init_stack(&s);

    for(i=0;i<n;i++){
        ch=in[i];
        switch(ch){         //��ȣ�� �˻�
    case '(':case'[':case'{':   //���� ��ȣ���
        push(&s,ch);         //���ÿ� ����
        break;
    case ')': case']':case'}':     //�ݴ� ��ȣ���
        if(is_empty(&s)) return 0;    //������ ����ִ��� �˻�
        else {
            open_ch=pop(&s);   //������ ���� �����ִ� ��ȣ�� ������ �ݴ°�ȣ�� �´��� ��
            if((open_ch=='('&&ch !=')')||(open_ch=='['&&ch !=']')||(open_ch=='{'&&ch !='}')){
                return 0;
            }
            break;
        }
        }
    }
    if(!is_empty(&s))return 0;  //���ڿ��� ���ٸ� 0
    return 1;  //�� �̿ܿ��� ��ȣ�˻� ���� 1
}

int prec(char op)   //�������� �켱���� ����
{
    switch(op){
    case '(': case ')':case '[': case']': case '{': case '}': return 0;
    case '+': case '-': return 1;
    case '*': case '/': return 2;
    }
    return -1;
}

void infix_to_postfix(char exp[],char post[])
{
    int i=0;
    int j=0;
    char ch,top_op;  //�Է¹��� ���ڸ� �ϳ��� �� ,top_op�� ���� ���� ��
    int len=strlen(exp);
    StackType s;
    init_stack(&s);
    for(i=0;i<len;i++){
        ch=exp[i];
        switch(ch){
    case '+': case '-': case '*': case '/':  //��ȣ �����ڰ� �ƴҰ��
        while(!is_empty(&s)&&(prec(ch)<=prec(peek(&s))))   //������ ������� �ʰ� ���翬���ڰ� ������ ������ �����ں��� �켱������ ���ٸ�(ex ���ÿ� * ,������+��� �켱������ �����Ƿ� ������ *�����ڸ� ���  )
         {
             post[j]=pop(&s);
             j++;
         }
        push(&s,ch);    //���ÿ� ���� �����ڸ� ����
        break;
    case '(': case'[': case '{': //���� ��ȣ�� ���
        push(&s,ch);    //���ÿ� ���� �����ڸ� ����
        break;
    case ')': case ']': case '}':   //�ݴ� ��ȣ�� ���
        top_op=pop(&s);               //top_op�� ������ ���� �� �����ڸ� ������ ����
        while((ch==')'&&top_op !='(')||(ch==']'&&top_op!='[')||(ch=='}'&&top_op!='{')){  //���� �����ڿ� �´� ���� ��ȣ�� ���ö� ����
                post[j]=top_op;
                j++;
                top_op=pop(&s);    //������ ���� ���� top�� ����
        }
        break;
    default:   //�����ϰ��
        post[j]=ch;
        j++;
        break;
        }
    }
    while(!is_empty(&s))  //���ÿ� �����ڰ� �����ִ°�� (ex ������ �������� ���ڷ� ������� ���ÿ� �����ڰ� �������� �� ����)
    {
         post[j]=pop(&s);
         j++;
    }
    post[j]='\0';
}

int main()
{
    int result;
    char s[MAX_STACK_SIZE];
    char post[MAX_STACK_SIZE];
    printf("�����Է� :");
    scanf("%[^\n]s",s); //���������Ͽ� ���ڿ� ������
    if(check_matching(s)==1)
        printf("%s \n��ȣ�˻�==>���� \n",s);
    else
        printf("%s \n��ȣ�˻�==>���� \n",s);
    infix_to_postfix(s,post);
    printf("�������� ǥ��==>%s\n",post);
    result=eval(post);
    printf("������==> %d\n",result);
    return 0;
}

