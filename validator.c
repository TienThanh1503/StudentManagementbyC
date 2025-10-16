#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_STUDENTS 100

typedef struct {
    char ID[20];
    char name[100];
    char className[20];
    double score;
    char dob[20];
    int attendance;
    char email[100];
    char gender[10];
} Student;

// Kiểm tra có ký tự đặc biệt hay không
bool hasSpecialChar(const char *str, const char *except) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isalpha(str[i]) && strchr(except, str[i]) == NULL)
            return true;
    }
    return false;
}

// Viết hoa chữ cái đầu của mỗi từ
void capitalize(char *str) {
    int i = 0;
    bool newWord = true;
    while (str[i]) {
        str[i] = tolower(str[i]);
        if (newWord && isalpha(str[i])) {
            str[i] = toupper(str[i]);
            newWord = false;
        } else if (isspace(str[i])) {
            newWord = true;
        }
        i++;
    }
}

// Căn giữa chuỗi trong độ rộng nhất định
void center(const char *s, int width, char *result) {
    int len = strlen(s);
    if (width <= len) {
        strcpy(result, s);
        return;
    }
    int pad = (width - len) / 2;
    sprintf(result, "%*s%s%*s", pad, "", s, width - len - pad, "");
}

// Hàm nhập ID (kiểm tra trùng và ký tự đặc biệt)
void inputID(Student students[], int count, char *id) {
    bool check;
    do {
        check = false;
        printf("Enter ID: ");
        fgets(id, 20, stdin);
        id[strcspn(id, "\n")] = '\0';

        if (hasSpecialChar(id, " 0123456789")) {
            printf("ID cannot contain special characters!\n");
            check = true;
        }
        if (strlen(id) > 10) {
            printf("ID is too long! Please enter a shorter one.\n");
            check = true;
        }
        for (int i = 0; i < count; i++) {
            if (strcasecmp(students[i].ID, id) == 0) {
                printf("ID %s already exists in the system!\n", id);
                check = true;
                break;
            }
        }
    } while (check);
    for (int i = 0; id[i]; i++)
        id[i] = toupper(id[i]);
}

// Nhập tên
void inputName(char *name) {
    bool check;
    do {
        check = false;
        printf("Enter Name: ");
        fgets(name, 100, stdin);
        name[strcspn(name, "\n")] = '\0';
        if (strlen(name) > 50) {
            printf("Name is too long!\n");
            check = true;
        }
        if (hasSpecialChar(name, " ")) {
            printf("Name cannot contain special characters or numbers!\n");
            check = true;
        }
    } while (check);
    capitalize(name);
}

// Nhập lớp
void inputClass(char *className) {
    bool check;
    do {
        check = false;
        printf("Enter Class: ");
        fgets(className, 20, stdin);
        className[strcspn(className, "\n")] = '\0';
        if (hasSpecialChar(className, "0123456789")) {
            printf("Class cannot contain special characters!\n");
            check = true;
        }
        if (strlen(className) > 8) {
            printf("Class name is too long!\n");
            check = true;
        }
    } while (check);
    for (int i = 0; className[i]; i++)
        className[i] = toupper(className[i]);
}

// Nhập điểm
double inputScore() {
    char buf[20];
    double res = 0;
    bool check;
    do {
        check = false;
        printf("Enter Score: ");
        fgets(buf, 20, stdin);
        buf[strcspn(buf, "\n")] = '\0';

        for (int i = 0; buf[i]; i++) {
            if (!isdigit(buf[i]) && buf[i] != '.' && buf[i] != '-') {
                check = true;
                break;
            }
        }
        if (check) {
            printf("Only digits and dots are allowed!\n");
            continue;
        }

        res = atof(buf);
        if (res < 0 || res > 10) {
            printf("Score must be between 0 and 10!\n");
            check = true;
        }
    } while (check);
    return res;
}

// Nhập ngày sinh
void inputDOB(char *dob) {
    bool check;
    int day, month, year;
    do {
        check = false;
        printf("Enter Date of Birth (dd/mm/yyyy): ");
        fgets(dob, 20, stdin);
        dob[strcspn(dob, "\n")] = '\0';

        if (sscanf(dob, "%d/%d/%d", &day, &month, &year) != 3) {
            printf("Invalid format! Must be dd/mm/yyyy\n");
            check = true;
            continue;
        }

        if (month < 1 || month > 12) {
            printf("Invalid month!\n");
            check = true;
        } else if (year < 0) {
            printf("Invalid year!\n");
            check = true;
        } else {
            int maxDay = 31;
            if (month == 4 || month == 6 || month == 9 || month == 11)
                maxDay = 30;
            else if (month == 2) {
                bool leap = (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
                maxDay = leap ? 29 : 28;
            }
            if (day < 1 || day > maxDay) {
                printf("Invalid day!\n");
                check = true;
            }
        }
    } while (check);
}

// Nhập điểm danh
int inputAttendance() {
    char buf[10];
    int val;
    bool check;
    do {
        check = false;
        printf("Enter Attendance: ");
        fgets(buf, 10, stdin);
        buf[strcspn(buf, "\n")] = '\0';
        for (int i = 0; buf[i]; i++) {
            if (!isdigit(buf[i])) {
                printf("Attendance must be numeric!\n");
                check = true;
                break;
            }
        }
        if (!check)
            val = atoi(buf);
        if (val > 999) {
            printf("Attendance is too large!\n");
            check = true;
        }
    } while (check);
    return val;
}

// Nhập email
void inputEmail(char *email) {
    bool check;
    do {
        check = false;
        printf("Enter email: ");
        fgets(email, 100, stdin);
        email[strcspn(email, "\n")] = '\0';
        if (strlen(email) == 0) {
            printf("Email cannot be empty!\n");
            check = true;
        } else if (!strchr(email, '@') || !strchr(email, '.')) {
            printf("Invalid email format!\n");
            check = true;
        }
    } while (check);
}

// Nhập giới tính
void inputGender(char *gender) {
    bool check;
    char temp[20];
    do {
        check = false;
        printf("Enter gender (Male/Female/Other): ");
        fgets(temp, 20, stdin);
        temp[strcspn(temp, "\n")] = '\0';
        for (int i = 0; temp[i]; i++) temp[i] = tolower(temp[i]);
        if (strcmp(temp, "male") == 0 || strcmp(temp, "m") == 0)
            strcpy(gender, "Male");
        else if (strcmp(temp, "female") == 0 || strcmp(temp, "f") == 0)
            strcpy(gender, "Female");
        else if (strcmp(temp, "other") == 0 || strcmp(temp, "o") == 0)
            strcpy(gender, "Other");
        else {
            printf("Invalid gender! Please enter Male, Female or Other.\n");
            check = true;
        }
    } while (check);
}

// ---- Demo sử dụng ----
int main() {
    Student students[MAX_STUDENTS];
    int count = 0;

    inputID(students, count, students[count].ID);
    inputName(students[count].name);
    inputClass(students[count].className);
    students[count].score = inputScore();
    inputDOB(students[count].dob);
    students[count].attendance = inputAttendance();
    inputEmail(students[count].email);
    inputGender(students[count].gender);

    printf("\n--- Student Information ---\n");
    printf("ID: %s\nName: %s\nClass: %s\nScore: %.2f\nDOB: %s\nAttendance: %d\nEmail: %s\nGender: %s\n",
           students[count].ID, students[count].name, students[count].className,
           students[count].score, students[count].dob, students[count].attendance,
           students[count].email, students[count].gender);

    return 0;
}
