#include<stdio.h>
#include<ctype.h>
#include<string.h>

int main(){
	char expression[100];
	char nexp[100];
	printf("Enter the Expression:");
	fgets(expression, sizeof(expression), stdin);
	expression[strcspn(expression, "\n")] = '\0';
	int j=0;
	int i;
	
	for(i = 0; expression[i] != '\0'; i++) {
        if (!isspace(expression[i])) {
            nexp[j++] = expression[i];
        }
    }
    nexp[j]='\0';
    
    for(i=0; nexp[i]!='\0';i++){
    	if(!isdigit(nexp[i]) && nexp[i]!='+' && nexp[i]!='-' && nexp[i]!='*'  && nexp[i]!='/' ){
    		printf("Error: invalid expression \n");
    		return 0;
		}
	}
//    printf("Original: %s\n", expression);
//    printf("Unspaced expresssion is : %s\n", nexp);
	
	int numbers[100];
	int operators[100];
	int n=0, o=0;
	
	for(i=0;nexp[i]!='\0';){
		if(isdigit(nexp[i])){
			int num=0;
			while(isdigit(nexp[i])){
				num = num*10 + (nexp[i] - '0');
				i++;
			}
			numbers[n++]=num;	
		}
		else{
			operators[o++]=nexp[i];
			i++;
		}
	}
	int result;
	for(i=0;i<o;i++){
		if(operators[i]=='*' || operators[i] == '/'){
			if (operators[i] == '/' && numbers[i+1] == 0) {
            printf("Error: division by zero \n");
            return 0;
		}
		if (operators[i] == '*'){
			result=numbers[i]*numbers[i+1];
		}
		else {
			result=numbers[i]/numbers[i+1];
		}
		numbers[i]=result;
		for (j = i+1; j < n-1; j++){
			numbers[j] = numbers[j+1];}
        for (j = i; j < o-1; j++){
        	operators[j] = operators[j+1];}
        n--; o--; i--;
		}
	}
	
	result=numbers[0];
	for (i = 0; i < o; i++) {
        if (operators[i] == '+'){
			result += numbers[i+1];}
        else result -= numbers[i+1];
    }
    printf("Result: %d\n", result);
    return 0;
}
