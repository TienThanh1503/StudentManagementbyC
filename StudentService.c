#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// ======================================================
// Cấu trúc Student
// ======================================================
typedef struct {
    char ID[20];
    char Name[60];
    char gender[15];
    char Class[15];
    char DOB[15];
    char email[60];
    double Score;
} Student;

// ======================================================
// Các hàm nhập dữ liệu (thay cho Validator.java)
// ======================================================

char* inputString(const char* prompt, char* buffer, int size) {
    printf("%s", prompt);
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    return buffer;
}

char* ID(Student* students, int count) {
    static char id[20];
    inputString("Enter student ID: ", id, sizeof(id));
    return id;
}

char* Name() {
    static char name[60];
    inputString("Enter student name: ", name, sizeof(name));
    return name;
}

char* gender() {
    static char g[15];
    inputString("Enter gender: ", g, sizeof(g));
    return g;
}

char* Class() {
    static char c[15];
    inputString("Enter class: ", c, sizeof(c));
    return c;
}

char* DOB() {
    static char dob[15];
    inputString("Enter date of birth (dd/mm/yyyy): ", dob, sizeof(dob));
    return dob;
}

char* email() {
    static char e[60];
    inputString("Enter email: ", e, sizeof(e));
    return e;
}

double Score() {
    double s;
    printf("Enter score: ");
    scanf("%lf", &s);
    getchar(); // clear newline
    return s;
}

// ======================================================
//  addStudent()
// ======================================================
Student addStudent(Student* students, int count) {
    Student s;
    strcpy(s.ID, ID(students, count));
    strcpy(s.Name, Name());
    strcpy(s.gender, gender());
    strcpy(s.Class, Class());
    strcpy(s.DOB, DOB());
    strcpy(s.email, email());
    s.Score = Score();
    return s;
}

// ======================================================
//  displayStudents()
// ======================================================
void displayStudents(char* option, char* data, char* comparisonOperator, Student* s, int size) {
    int maxLengthOfNames = 8;
    int maxLengthOfEmails = 12;

    for (int i = 0; i < size; i++) {
        int lenName = strlen(s[i].Name);
        int lenEmail = strlen(s[i].email);
        if (lenName > maxLengthOfNames) maxLengthOfNames = lenName;
        if (lenEmail > maxLengthOfEmails) maxLengthOfEmails = lenEmail;
    }

    int totalWidth = 80 + maxLengthOfNames + maxLengthOfEmails;

    for (int i = 0; i < totalWidth; i++) printf("=");
    printf("\n");

    printf("| %-5s | %-10s | %-*s | %-8s | %-10s | %-14s | %-*s | %-8s |\n",
           "No.", "ID", maxLengthOfNames, "Name", "Gender", "Class",
           "Date of Birth", maxLengthOfEmails, "Email", "Score");

    for (int i = 0; i < totalWidth; i++) printf("-");
    printf("\n");

    for (int i = 0; i < size; i++) {
        printf("| %-5d | %-10s | %-*s | %-8s | %-10s | %-14s | %-*s | %-8.2f |\n",
               i + 1, s[i].ID, maxLengthOfNames, s[i].Name, s[i].gender,
               s[i].Class, s[i].DOB, maxLengthOfEmails, s[i].email, s[i].Score);
    }

    for (int i = 0; i < totalWidth; i++) printf("=");
    printf("\n");
}

// ======================================================
//  search()
// ======================================================
void search(Student* students, int count) {
    char keyword[100];
    int found = 0;

    printf("Enter keyword to search (ID, Name, Class, or Email): ");
    fgets(keyword, sizeof(keyword), stdin);
    keyword[strcspn(keyword, "\n")] = 0;

    printf("\n======================= SEARCH RESULT =======================\n");
    printf("| %-5s | %-10s | %-20s | %-8s | %-8s | %-14s | %-20s | %-6s |\n",
           "No.", "ID", "Name", "Gender", "Class", "Date of Birth", "Email", "Score");
    printf("----------------------------------------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < count; i++) {
        if (strstr(students[i].ID, keyword) != NULL ||
            strstr(students[i].Name, keyword) != NULL ||
            strstr(students[i].Class, keyword) != NULL ||
            strstr(students[i].email, keyword) != NULL) {

            found++;
            printf("| %-5d | %-10s | %-20s | %-8s | %-8s | %-14s | %-20s | %-6.2f |\n",
                   found,
                   students[i].ID,
                   students[i].Name,
                   students[i].gender,
                   students[i].Class,
                   students[i].DOB,
                   students[i].email,
                   students[i].Score);
        }
    }

    if (found == 0) {
        printf("No student found matching '%s'\n", keyword);
    }

    printf("=============================================================================================================================\n");
}

// ======================================================
//  deleteStudent()
// ======================================================
int deleteStudent(Student* students, int* count) {
    char id[20];
    int found = 0;

    printf("Enter Student ID to delete: ");
    fgets(id, sizeof(id), stdin);
    id[strcspn(id, "\n")] = 0;

    for (int i = 0; i < *count; i++) {
        if (strcmp(students[i].ID, id) == 0) {
            found = 1;

            printf("\nStudent found: %s (%s)\n", students[i].Name, students[i].ID);
            printf("Are you sure you want to delete this student? (Y/N): ");
            char confirm;
            scanf(" %c", &confirm);
            getchar();

            if (confirm == 'Y' || confirm == 'y') {
                for (int j = i; j < *count - 1; j++) {
                    students[j] = students[j + 1];
                }
                (*count)--;

                printf("\n✅ Student with ID %s deleted successfully.\n", id);
            } else {
                printf("\n❎ Deletion canceled.\n");
            }
            break;
        }
    }

    if (!found) {
        printf("\n⚠️ No student found with ID: %s\n", id);
    }

    return found;
}

// ======================================================
//  update()
// ======================================================
void update(Student* students, int count) {
    int idx = 0;
    char idSearch[20];   // 🔹 đổi tên để tránh trùng với hàm ID()
    int check = 0;

    do {
        check = 0;
        printf("Enter ID to update student's information: ");
        fgets(idSearch, sizeof(idSearch), stdin);
        idSearch[strcspn(idSearch, "\n")] = 0;

        for (int i = 0; idSearch[i] != '\0'; i++) {
            if (!isalnum(idSearch[i])) {
                printf("ID cannot contain special characters!\n");
                check = 1;
                break;
            }
        }

        if (strlen(idSearch) > 10) {
            printf("ID is too long! Please enter a shorter one.\n");
            check = 1;
        }

    } while (check);

    while (idx < count) {
        if (strcasecmp(students[idx].ID, idSearch) == 0) break;
        idx++;
    }

    if (idx == count) {
        printf("ID %s NOT FOUND! \nUpdate failed.\n", idSearch);
        return;
    }

    printf("\nCurrent student info:\n");
    printf("ID: %s\nName: %s\nGender: %s\nClass: %s\nDOB: %s\nEmail: %s\nScore: %.2f\n",
           students[idx].ID, students[idx].Name, students[idx].gender,
           students[idx].Class, students[idx].DOB, students[idx].email, students[idx].Score);

    printf("\n=== UPDATE MENU ===\n");
    printf("1. ID\n");
    printf("2. Name\n");
    printf("3. Class\n");
    printf("4. Score\n");
    printf("5. Date of Birth\n");
    printf("6. Gender\n");
    printf("7. Email\n");
    printf("Enter your choice: ");

    char choice[10];
    fgets(choice, sizeof(choice), stdin);
    choice[strcspn(choice, "\n")] = 0;

    switch (choice[0]) {
        case '1': 
			strcpy(students[idx].ID, ID(students, count)); 
			break;   
        case '2': 
			strcpy(students[idx].Name, Name()); 
			break;
        case '3': 
			strcpy(students[idx].Class, Class()); 
			break;
        case '4': 
			students[idx].Score = Score(); 
			break;
        case '5': 
			strcpy(students[idx].DOB, DOB()); 
			break;
        case '6': 
			strcpy(students[idx].gender, gender()); 
			break;
        case '7': 
			strcpy(students[idx].email, email()); 
			break;
        default: printf("Invalid choice!\n"); return;
    }

    printf("Update successfully!\n");
}

// ======================================================
//  sort() + comparator functions
// ======================================================
int compareIDAsc(const void* a, const void* b){
 	return strcasecmp(((Student*)a)->ID, ((Student*)b)->ID);
}
int compareIDDesc(const void* a, const void* b){
 	return strcasecmp(((Student*)b)->ID, ((Student*)a)->ID);
}
int compareNameAsc(const void* a, const void* b){
 	return strcasecmp(((Student*)a)->Name, ((Student*)b)->Name);
}
int compareNameDesc(const void* a, const void* b){
 	return strcasecmp(((Student*)b)->Name, ((Student*)a)->Name);
}
int compareClassAsc(const void* a, const void* b){
 	return strcasecmp(((Student*)a)->Class, ((Student*)b)->Class);
}
int compareClassDesc(const void* a, const void* b){
 	return strcasecmp(((Student*)b)->Class, ((Student*)a)->Class);
}
int compareScoreAsc(const void* a, const void* b){
	 double diff=((Student*)a)->Score-((Student*)b)->Score;
	 return (diff>0)-(diff<0);
}
int compareScoreDesc(const void* a, const void* b){
	 double diff=((Student*)b)->Score-((Student*)a)->Score;
	 return (diff>0)-(diff<0);
}
int compareDOBAsc(const void* a, const void* b){
 	return strcmp(((Student*)a)->DOB, ((Student*)b)->DOB);
}
int compareDOBDesc(const void* a, const void* b){
 	return strcmp(((Student*)b)->DOB, ((Student*)a)->DOB);
}
int compareGenderAsc(const void* a, const void* b){
 	return strcasecmp(((Student*)a)->gender, ((Student*)b)->gender);
}
int compareGenderDesc(const void* a, const void* b){
 	return strcasecmp(((Student*)b)->gender, ((Student*)a)->gender);
}
int compareEmailAsc(const void* a, const void* b){
 	return strcasecmp(((Student*)a)->email, ((Student*)b)->email);
}
int compareEmailDesc(const void* a, const void* b){
 	return strcasecmp(((Student*)b)->email, ((Student*)a)->email);
}

void sort(Student* students, int count) {
    printf("\n=== SORT STUDENTS ===\n1. ID\n2. Name\n3. Class\n4. Score\n5. DOB\n6. Gender\n7. Email\n");
    printf("Enter your choice: ");
    char choice[10]; 
	fgets(choice,sizeof(choice),stdin); 
	choice[strcspn(choice,"\n")]=0;

    printf("\nSort order:\n1. Ascending\n2. Descending\nEnter your choice: ");
    char option[10]; 
	fgets(option,sizeof(option),stdin); 
	option[strcspn(option,"\n")]=0;

    Student s_copy[1000];
    memcpy(s_copy, students, sizeof(Student)*count);

    switch(choice[0]){
        case '1': 
			qsort(s_copy,count,sizeof(Student),(option[0]=='1')?compareIDAsc:compareIDDesc);
			break;
        case '2': 
			qsort(s_copy,count,sizeof(Student),(option[0]=='1')?compareNameAsc:compareNameDesc);
			break;
        case '3': 
			qsort(s_copy,count,sizeof(Student),(option[0]=='1')?compareClassAsc:compareClassDesc);
			break;
        case '4': 
			qsort(s_copy,count,sizeof(Student),(option[0]=='1')?compareScoreAsc:compareScoreDesc);
			break;
        case '5': 
			qsort(s_copy,count,sizeof(Student),(option[0]=='1')?compareDOBAsc:compareDOBDesc);
			break;
        case '6': 
			qsort(s_copy,count,sizeof(Student),(option[0]=='1')?compareGenderAsc:compareGenderDesc);
			break;
        case '7': 
			qsort(s_copy,count,sizeof(Student),(option[0]=='1')?compareEmailAsc:compareEmailDesc);
			break;
        default: printf("Invalid choice!\n"); return;
    }
    displayStudents("all","","",s_copy,count);
}

// ======================================================
//  MAIN TEST
// ======================================================
int main() {
    Student students[3];
    int count = 0;
    int running = 1;
    char choice[10];

    while (running) {
        printf("\n=========== STUDENT MANAGEMENT MENU ===========\n");
        printf("1. Add student\n");
        printf("2. Display students\n");
        printf("3. Search student\n");
        printf("4. Delete student\n");
        printf("5. Update student\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        fgets(choice, sizeof(choice), stdin);
        choice[strcspn(choice, "\n")] = 0;

        switch (choice[0]) {
            case '1':
                students[count++] = addStudent(students, count);
                break;
            case '2':
                displayStudents("all", "", "", students, count);
                break;
            case '3':
                search(students, count);
                break;
            case '4':
                deleteStudent(students, &count);
                break;
            case '5':
                update(students, count);
                break;
            case '6':
                running = 0;
                break;
            default:
                printf("Invalid choice!\n");
        }
    }
    return 0;
}
