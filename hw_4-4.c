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
        fprintf(stderr,"스택 포화 에러\n");
        return;
    }
    else s->data[++(s->top)]=item;
}

element pop(StackType *s)
{
   if(is_empty(s)){
        fprintf(stderr,"스택 공백 에러\n");
        exit(1);
    }
    else return s->data[(s->top)--];
}

element peek(StackType *s)
{
   if(is_empty(s)){
        fprintf(stderr,"스택 공백 에러\n");
        exit(1);
    }
    else return s->data[(s->top)];
}

int eval(char exp[])    //(스택에는 숫자를 저장 )
{
    int op1,op2,value,i=0;
    int len=strlen(exp);
    char ch;
    StackType s;

    init_stack(&s);   //스택 초기화
    for(i=0;i<len;i++){
        ch=exp[i];           //입력한 수식
        if(isdigit(ch)){   //입력한 수식이 숫자일경우
            value=ch-'0';  //아스키코드 0을 빼 문자형을 숫자형으로 바꿈
            push(&s,value);//스택에 숫자를 저장
        }
        else{
            op2=pop(&s);  //연산자일 경우
            op1=pop(&s);   //op2,1에 스택 상위의 숫자 2개를 저장
            switch(ch){
            case '+':push(&s,op1+op2);break;  //덧셈
            case '-':push(&s,op1-op2);break;
            case '*':push(&s,op1*op2);break;
            case '/':push(&s,op1/op2);break;
            }
        }
    }
    return pop(&s); //마지막남은 스택==결과
}

int check_matching(const char *in)
{
    StackType s;
    char ch,open_ch;    //ch입력받은 값을 하나씩 확인하기 위한 변수, open ch닫는 괄호를 만났을때 스택에서 꺼내오는 여는 괄호
    int i,n=strlen(in);
    init_stack(&s);

    for(i=0;i<n;i++){
        ch=in[i];
        switch(ch){         //괄호를 검사
    case '(':case'[':case'{':   //여는 괄호라면
        push(&s,ch);         //스택에 저장
        break;
    case ')': case']':case'}':     //닫는 괄호라면
        if(is_empty(&s)) return 0;    //스택이 비어있는지 검사
        else {
            open_ch=pop(&s);   //스택의 제일 위에있는 괄호를 꺼내어 닫는괄호와 맞는지 비교
            if((open_ch=='('&&ch !=')')||(open_ch=='['&&ch !=']')||(open_ch=='{'&&ch !='}')){
                return 0;
            }
            break;
        }
        }
    }
    if(!is_empty(&s))return 0;  //문자열이 없다면 0
    return 1;  //이 이외에는 괄호검사 성공 1
}

int prec(char op)   //연산자의 우선순위 설정
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
    char ch,top_op;  //입력받은 문자를 하나씩 비교 ,top_op는 스택 제일 위
    int len=strlen(exp);
    StackType s;
    init_stack(&s);
    for(i=0;i<len;i++){
        ch=exp[i];
        switch(ch){
    case '+': case '-': case '*': case '/':  //괄호 연산자가 아닐경우
        while(!is_empty(&s)&&(prec(ch)<=prec(peek(&s))))   //스택이 비어있지 않고 현재연산자가 스택의 제일위 연산자보다 우선순위가 낮다면(ex 스택에 * ,연산자+라면 우선순위가 낮으므로 스택의 *연산자를 출력  )
         {
             post[j]=pop(&s);
             j++;
         }
        push(&s,ch);    //스택에 현재 연산자를 저장
        break;
    case '(': case'[': case '{': //여는 괄호일 경우
        push(&s,ch);    //스택에 현재 연산자를 저장
        break;
    case ')': case ']': case '}':   //닫는 괄호일 경우
        top_op=pop(&s);               //top_op에 스택의 제일 위 연산자를 꺼내어 저장
        while((ch==')'&&top_op !='(')||(ch==']'&&top_op!='[')||(ch=='}'&&top_op!='{')){  //현재 연산자에 맞는 여는 괄호가 나올때 까지
                post[j]=top_op;
                j++;
                top_op=pop(&s);    //스택의 제일 위를 top에 저장
        }
        break;
    default:   //숫자일경우
        post[j]=ch;
        j++;
        break;
        }
    }
    while(!is_empty(&s))  //스택에 연산자가 남아있는경우 (ex 수식의 마지막이 숫자로 끝날경우 스택에 연산자가 남아있을 수 있음)
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
    printf("수식입력 :");
    scanf("%[^\n]s",s); //공백포함하여 문자열 저장방법
    if(check_matching(s)==1)
        printf("%s \n괄호검사==>성공 \n",s);
    else
        printf("%s \n괄호검사==>실패 \n",s);
    infix_to_postfix(s,post);
    printf("후위수식 표현==>%s\n",post);
    result=eval(post);
    printf("연산결과==> %d\n",result);
    return 0;
}

