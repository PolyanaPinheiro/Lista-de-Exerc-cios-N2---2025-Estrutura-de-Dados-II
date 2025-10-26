/*
 * Resolucao da Questao 03
 * Avaliador de Notacao Polonesa Reversa (RPN)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


typedef struct node {
    double value;
    struct node *prox;
} Node;

typedef struct stack {
    Node *top;
} Stack;


Stack* createStack() {
    Stack* stack = (Stack*) malloc(sizeof(Stack));
    stack->top = NULL;
    return stack;
}


int isEmpty(Stack* stack) {
    return (stack->top == NULL);
}


void push(Stack* stack, double value) {
    Node* newNode = (Node*) malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Erro: Falha na alocacao de memoria (push).\n");
        exit(1);
    }
    newNode->value = value;
    newNode->prox = stack->top;
    stack->top = newNode;
}

double pop(Stack* stack) {

    
    Node* temp = stack->top;
    double value = temp->value;
    stack->top = temp->prox;
    free(temp);
    return value;
}

void freeStack(Stack* stack) {
    while (!isEmpty(stack)) {
        pop(stack);
    }
    free(stack);
}

double evaluateRPN(char* expression) {
    Stack* stack = createStack();
    
    char* token = strtok(expression, " ");

    while (token != NULL) {
        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) {
            push(stack, atof(token));
        }
        else if (strchr("+-*/", token[0]) && strlen(token) == 1) {
            if (isEmpty(stack)) {
                printf("Erro: Expressao malformada (operandos insuficientes para o operador '%s').\n", token);
                freeStack(stack);
                exit(1);
            }
            double val2 = pop(stack);

            if (isEmpty(stack)) {
                printf("Erro: Expressao malformada (operandos insuficientes para o operador '%s').\n", token);
                freeStack(stack);
                exit(1);
            }
            double val1 = pop(stack);

            double result;
            
            switch (token[0]) {
                case '+':
                    result = val1 + val2;
                    break;
                case '-':
                    result = val1 - val2;
                    break;
                case '*':
                    result = val1 * val2;
                    break;
                case '/':
                    if (val2 == 0|| val1==0) {
                        printf("Erro: Tentativa de divisao por zero.\n");
                        freeStack(stack);
                        exit(1);
                    }
                    result = val1 / val2;
                    break;
            }
            push(stack, result);
        }
        else {
             printf("Erro: Token desconhecido '%s'.\n", token);
             freeStack(stack);
             exit(1);
        }
        
        token = strtok(NULL, " ");
    }


    double finalResult = pop(stack);


    if (!isEmpty(stack)) {
        printf("Erro: Expressao malformada (operandos extras).\n");
        freeStack(stack);
        exit(1);
    }

    freeStack(stack);
    return finalResult;
}


int main(int argc, char *argv[]) {
    // 
    if (argc != 2) {
        printf("Uso: %s \"<expressao RPN>\"\n", argv[0]);
        printf("Exemplo: %s \"10 5 + 2 *\"\n", argv[0]);
        return 1;
    }


    char* expression = (char*) malloc(strlen(argv[1]) + 1);
    strcpy(expression, argv[1]);

    double result = evaluateRPN(expression);


    printf("Resultado: %.2f\n", result);

    free(expression);
    return 0;
}