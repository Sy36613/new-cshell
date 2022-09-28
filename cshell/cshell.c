#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

typedef struct {
	char *name;
	struct tm time;
	int value;
} Command;

void strip_newline(char *str, int size) {
	for (int i = 0; i < size; i++) {
		if (str[i] == '\n' || str[i] == '\r') {
			str[i] = '\0';
			return;
		}
	}
}

void setColor(int color) {
	if (color == 7)
		printf("\033[0;37m");
	else if (color == 1)
		printf("\033[0;31m");
	else if (color == 2)
		printf("\033[0;32m");
	else if (color == 4)
		printf("\033[0;34m");
}

int main(int argc, char *argv[]) {
	if (argc == 1) { // interactive mode
	char line[256];
	char *pch;
	char var[100];
	Command cs[100];
	int c = 0, color = 7;
	printf("cshell$ ");
	while (fgets(line, 256, stdin)) {
		setColor(color);
		strip_newline(line, 256);
		pch = strtok(line, " ");
		time_t rawtime;
		time(&rawtime);
		if (strcmp(pch, "exit") == 0) {
			printf("Bye!\n");
			return 0;
		} else if (strcmp(pch, "print") == 0) {
			cs[c].name = "print";
			cs[c].value = 0;
			cs[c].time = *(localtime(&rawtime));
			pch = strtok(NULL, "\0");
			if (pch == NULL) {
				printf("\n");
			} else if (pch[0] == '$')
				printf("%s\n", var);
			else
				printf("%s\n", pch);
			c += 1;
		} else if (strcmp(pch, "theme") == 0) {
			pch = strtok(NULL, "\0");
			cs[c].name = "theme";
			cs[c].value = 0;
			cs[c].time = *(localtime(&rawtime));
			if (pch == NULL) {
				printf("unsupported theme\n");
				cs[c].value = -1;
			} else if (strcmp(pch, "red") == 0) {
				printf("\033[0;31m");
				color = 1;
			} else if (strcmp(pch, "green") == 0) {
				printf("\033[0;32m");
				color = 2;
			} else if (strcmp(pch, "blue") == 0) {
				printf("\033[0;34m");
				color = 4;
			} else {
				printf("unsupported theme\n");
				cs[c].value = -1;
			}
			c += 1;
		} else if (strcmp(pch, "ls") == 0) {
			system("ls");
			cs[c].name = "ls";
			cs[c].value = 0;
			cs[c].time = *(localtime(&rawtime));
			c += 1;
		} else if (strcmp(pch, "log") == 0) {
			for (int i = 0; i < c; i++) {
				setColor(color);
				printf("%s", asctime(&cs[i].time));
				setColor(color);
				printf(" %s %d\n", cs[i].name, cs[i].value);
			}
			cs[c].name = "log";
			cs[c].value = 0;
			cs[c].time = *(localtime(&rawtime));
			c += 1;
		} else if (strcmp(pch, "pwd") == 0) {
			system("pwd");
			cs[c].name = "pwd";
			cs[c].value = 0;
			cs[c].time = *(localtime(&rawtime));
			c += 1;
		} else if (strcmp(pch, "whoami") == 0) {
			system("whoami");
			cs[c].name = "whoami";
			cs[c].value = 0;
			cs[c].time = *(localtime(&rawtime));
			c += 1;
		} else if (pch[0] == '$') {
			cs[c].name = malloc(100);
			cs[c].value = 0;
			cs[c].time = *(localtime(&rawtime));
			char* pch2 = strtok(NULL, "\0");
			if (pch2 == NULL) {
				int i = 0;
				for (i = 0; i < 100; i++) {
					if (pch[i] == '=')
						break;
				}
				i += 1;
				for (int j = 0; j < 100; j++,i++) {
					var[j] = pch[i];
					if (pch[i] == '\0')
						break;
				}
				for (int i = 0; i < 100; i++) {
					cs[c].name[i] = pch[i];
					if (pch[i] == '\0')
						break;
				}
			} else {
				printf("Variable value expected\n");
				for (int i = 0; i < 100; i++) {
					cs[c].name[i] = line[i];
					if (line[i] == '\0')
						break;
				}
				cs[c].value = -1;
			}
			c += 1;
		} else {
			cs[c].name = pch;
			cs[c].value = -1;
			cs[c].time = *(localtime(&rawtime));
			c += 1;
			printf("Missing keyword or command, or permission problem\n");
		}
		printf("cshell$ ");
		printf("\033[0;37m");
	}

	return 0;
	}

	// script mode
	FILE *fptr = fopen(argv[1], "r");
	char *line;
	size_t len = 0;
	ssize_t ret = 0;
	char *pch;
	char var[100];
	Command cs[100];
	int c = 0, color = 7;
	while ((ret = getline(&line, &len, fptr)) != -1) {
		setColor(color);
		strip_newline(line, 256);
		if (line[0] == '\0') {
			printf("Bye!\n");
			return 0;
		}
		pch = strtok(line, " ");
		time_t rawtime;
		time(&rawtime);
		if (strcmp(pch, "print") == 0) {
			cs[c].name = "print";
			cs[c].value = 0;
			cs[c].time = *(localtime(&rawtime));
			pch = strtok(NULL, "\0");
			if (pch == NULL) {
				printf("\n");
			} else if (pch[0] == '$')
				printf("%s\n", var);
			else
				printf("%s\n", pch);
			c += 1;
		} else if (strcmp(pch, "theme") == 0) {
			pch = strtok(NULL, "\0");
			cs[c].name = "theme";
			cs[c].value = 0;
			cs[c].time = *(localtime(&rawtime));
			if (pch == NULL) {
				printf("unsupported theme\n");
				cs[c].value = -1;
			} else if (strcmp(pch, "red") == 0) {
				printf("\033[0;31m");
				color = 1;
			} else if (strcmp(pch, "green") == 0) {
				printf("\033[0;32m");
				color = 2;
			} else if (strcmp(pch, "blue") == 0) {
				printf("\033[0;34m");
				color = 4;
			} else {
				printf("unsupported theme\n");
				cs[c].value = -1;
			}
			c += 1;
		} else if (strcmp(pch, "ls") == 0) {
			system("ls");
			cs[c].name = "ls";
			cs[c].value = 0;
			cs[c].time = *(localtime(&rawtime));
			c += 1;
		} else if (strcmp(pch, "log") == 0) {
			for (int i = 0; i < c; i++) {
				setColor(color);
				printf("%s", asctime(&cs[i].time));
				setColor(color);
				printf(" %s %d\n", cs[i].name, cs[i].value);
			}
			cs[c].name = "log";
			cs[c].value = 0;
			cs[c].time = *(localtime(&rawtime));
			c += 1;
		} else if (strcmp(pch, "pwd") == 0) {
			system("pwd");
			cs[c].name = "pwd";
			cs[c].value = 0;
			cs[c].time = *(localtime(&rawtime));
			c += 1;
		} else if (strcmp(pch, "whoami") == 0) {
			system("whoami");
			cs[c].name = "whoami";
			cs[c].value = 0;
			cs[c].time = *(localtime(&rawtime));
			c += 1;
		} else if (pch[0] == '$') {
			cs[c].name = malloc(100);
			cs[c].value = 0;
			cs[c].time = *(localtime(&rawtime));
			char* pch2 = strtok(NULL, "\0");
			if (pch2 == NULL) {
				int i = 0;
				for (i = 0; i < 100; i++) {
					if (pch[i] == '=')
						break;
				}
				i += 1;
				for (int j = 0; j < 100; j++,i++) {
					var[j] = pch[i];
					if (pch[i] == '\0')
						break;
				}
				for (int i = 0; i < 100; i++) {
					cs[c].name[i] = pch[i];
					if (pch[i] == '\0')
						break;
				}
			} else {
				printf("Variable value expected\n");
				for (int i = 0; i < 100; i++) {
					cs[c].name[i] = line[i];
					if (line[i] == '\0')
						break;
				}
				cs[c].value = -1;
			}
			c += 1;
		} else {
			cs[c].name = pch;
			cs[c].value = -1;
			cs[c].time = *(localtime(&rawtime));
			c += 1;
			printf("Missing keyword or command, or permission problem\n");
		}
		printf("\033[0;37m");
	}
	fclose(fptr);
	if (line)
		free(line);
	return 0;
}
