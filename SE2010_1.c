#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#define MAX_LINE 1024
// =================== STRUCT DEFINITION ===================
typedef struct {
    char ID[20];
    char Name[50];
    char gender[10];
    char Class[20];
    char DOB[15];
    char email[50];
    double Score;
} Student;
typedef struct {
    Student *data;
    int size;
    int capacity;
} ArrayList;

// ========================= PROTOTYPES =========================
// --------------------- ArrayList functions --------------------
void initList(ArrayList *list);
void resizeList(ArrayList *list);
void addStudentToList(ArrayList *list, Student s);
void freeList(ArrayList *list);

// ----------internal utilities for Validator Functions------------
void read_line(char *buf, size_t size);
void trim_inplace(char *s);
char *dupstr(const char *s);
int equals_ignore_case(const char *a, const char *b);
void to_upper_inplace(char *s);
int id_exists(const Student *arr, size_t n, const char *id);
int email_exists(const Student *arr, size_t n, const char *email);
int only_digits_dot_dash(const char *s);
int only_digits_slash(const char *s);
int only_digits(const char *s);
int hasSpecialChar(const char *str, const char *except);
char *capitalize(const char *str_in);
char *center(const char *s_in, int width);
// ---------- Internal Utilities (now global, no static) ------------
int  str_icmp(const char *a, const char *b);
int  str_ieq(const char *a, const char *b);
int  contains_ignore_case(const char *hay, const char *needle);
void print_repeat(char ch, int count);
void removeAt(ArrayList *list, int index);
void clearList(ArrayList *list);
ArrayList copyList(const ArrayList *src);
void last_name(const char *full, char *out, size_t out_sz);
// --------------Internal utilities for FileService------------------
int   file_exists(const char *path);
int   ask_yes_no(const char *prompt);                   /* returns 1 if yes */
void  fprint_repeat(FILE *fp, char ch, int count);
void  compute_max_widths(const ArrayList *s, int *nameW, int *emailW);
void  sanitize_csv_field_remove_commas(const char *in, char *out, size_t outsz);
void  open_file_with_default_app(const char *path);     /* best-effort */

//------------------------qsort comparators---------------------------
int cmp_id_asc   (const void *a, const void *b);
int cmp_id_desc  (const void *a, const void *b);
int cmp_class_asc(const void *a, const void *b);
int cmp_class_desc(const void *a, const void *b);
int cmp_gender_asc(const void *a, const void *b);
int cmp_gender_desc(const void *a, const void *b);
int cmp_email_asc(const void *a, const void *b);
int cmp_email_desc(const void *a, const void *b);
int cmp_name_asc (const void *a, const void *b);
int cmp_name_desc(const void *a, const void *b);
int cmp_dob_asc  (const void *a, const void *b);
int cmp_dob_desc (const void *a, const void *b);
int cmp_score_asc(const void *a, const void *b);
int cmp_score_desc(const void *a, const void *b);

//----------------------Validator Functions-----------------------
char *ID(Student *students, size_t n_students, int option);
char *Name(void);
char *Class(void);
double Score(void);
char *DOB(void);
char *email(Student *students, size_t n_students, int option);
char *gender(void);
//-----------------------StudentService---------------------------
Student addStudent(ArrayList *students);
void displayStudents(const char *option, const char *data, const char *comparisonOperator, const ArrayList *s);
void search(ArrayList *s);
void delete(ArrayList *s);
void update(ArrayList *s);
void sort(ArrayList *s);
//-----------------------FileService-------------------------------
void saveFile(ArrayList *s);
void loadFile(ArrayList *s);
void saveTextFile(ArrayList *s);
void loadTextFile(ArrayList *s);
void saveCSVFile(ArrayList *s);
void loadCSVFile(ArrayList *s);
//----------------------Systems------------------------------------
void setting();
void history();
//-----------------operationHistory--------------------------------
void OperationHistory_init(void);
void OperationHistory_log(const char *action,
                          const char *target,
                          const char *desc,
                          const char *status);
void OperationHistory_close(void);
void OperationHistory_clear();
//--------------------Private state (internal)---------------------
static FILE *fw = NULL;  
static FILE *bw = NULL; 
static const char *FILE_NAME = "operationHistory.csv"; 
static int count = 0;    
static int initialized = 0;
static char description[256];

//----------------------------main----------------------------------
void showMenu(void);
void handleChoice(ArrayList *students, char *choice);


// ========================= MAIN =========================
int main() {
    ArrayList students;
    initList(&students);

    char choice[10];
    do {
        showMenu();
        fgets(choice, sizeof(choice), stdin);
        choice[strcspn(choice, "\n")] = 0;
        handleChoice(&students, choice);
    } while (strcmp(choice, "0") != 0);

    freeList(&students);
    return 0;
}

// ========================= DISPLAY MENU =========================
void showMenu() {
    printf("\n=== Student Management System ===\n");
    printf("1. Add Student\n");
    printf("2. Display Students\n");
    printf("3. Search\n");
    printf("4. Delete\n");
    printf("5. Update\n");
    printf("6. Sort\n");
    printf("7. Save Data\n");
    printf("8. Load Data\n");
    printf("9. Setting\n");
    printf("0. Exit\n");
    printf("Enter your choice: ");
}
// ========================= HANDLE CHOICE =========================
// prototype (placed before main)
void handleChoice(ArrayList *students, char *choice);

// implement
void handleChoice(ArrayList *students, char *choice) {
    switch (choice[0]) {
        case '1': {
            // addStudent takes ArrayList* and returns a Student
            Student s = addStudent(students);
            addStudentToList(students, s);
            printf("Add a student successfully!\n");
            break;
        }
        case '2':
            // displayStudents takes ArrayList*
            displayStudents("all", "", "", students);
            break;

        case '3':
            // search takes ArrayList*
            search(students);
            break;

        case '4':
            // the delete function in C is 'delete' (not deleteStudent)
            delete(students);
            break;

        case '5':
            // update takes ArrayList*
            update(students);
            break;

        case '6':
            // sort takes ArrayList*
            sort(students);
            break;

        case '7':
            // saveFile takes ArrayList*
            saveFile(students);
            break;

        case '8':
            // loadFile takes ArrayList*
            loadFile(students);
            break;
        case '9':
        	setting();
        	break;
		case '0':{
            char ans[10];
            printf("Do you want to save before exit? (y/n): ");
            read_line(ans, sizeof(ans));          // use existing utility function
            if (ans[0] == 'y' || ans[0] == 'Y') {
                saveFile(students);
            }
            printf("Exiting program...\n");
            break;
        }

        default:
            printf("Invalid choice!\n");
    }
}

// =====================ArrayList============================================
void initList(ArrayList *list) {
    list->size = 0;
    list->capacity = 10;
    list->data = (Student *)malloc(list->capacity * sizeof(Student));
}

void resizeList(ArrayList *list) {
    list->capacity *= 2;
    list->data = (Student *)realloc(list->data, list->capacity * sizeof(Student));
}

void addStudentToList(ArrayList *list, Student s) {
    if (list->size >= list->capacity)
        resizeList(list);
    list->data[list->size++] = s;
}

void freeList(ArrayList *list) {
    free(list->data);
    list->data = NULL;
    list->size = 0;
    list->capacity = 0;
}

// --------- Internal utilities for Validator Functions----------
void read_line(char *buf, size_t size) {
    if (!fgets(buf, (int)size, stdin)) { buf[0] = '\0'; return; }
    size_t n = strcspn(buf, "\r\n");
    buf[n] = '\0';
}
void trim_inplace(char *s) {
    // ltrim
    size_t i = 0, n = strlen(s);
    while (i < n && isspace((unsigned char)s[i])) i++;
    if (i > 0) memmove(s, s + i, n - i + 1);
    // rtrim
    n = strlen(s);
    while (n > 0 && isspace((unsigned char)s[n - 1])) n--;
    s[n] = '\0';
}
char *dupstr(const char *s) {
    size_t n = strlen(s);
    char *p = (char*)malloc(n + 1);
    if (!p) exit(1);
    memcpy(p, s, n + 1);
    return p;
}
int equals_ignore_case(const char *a, const char *b) {
    while (*a && *b) {
        int ca = tolower((unsigned char)*a++);
        int cb = tolower((unsigned char)*b++);
        if (ca != cb) return 0;
    }
    return *a == '\0' && *b == '\0';
}
void to_upper_inplace(char *s) {
    for (; *s; ++s) *s = (char)toupper((unsigned char)*s);
}
int id_exists(const Student *arr, size_t n, const char *id) {
    for (size_t i = 0; i < n; ++i) {
        if (equals_ignore_case(arr[i].ID, id)) return 1;
    }
    return 0;
}
int only_digits_dot_dash(const char *s) {
    if (*s == '\0') return 0;
    for (; *s; ++s) {
        if (!(isdigit((unsigned char)*s) || *s == '.' || *s == '-')) return 0;
    }
    return 1;
}
int only_digits_slash(const char *s) {
    if (*s == '\0') return 0;
    for (; *s; ++s) {
        if (!(isdigit((unsigned char)*s) || *s == '/')) return 0;
    }
    return 1;
}
int only_digits(const char *s) {
    if (*s == '\0') return 0;
    for (; *s; ++s) if (!isdigit((unsigned char)*s)) return 0;
    return 1;
}

int hasSpecialChar(const char *str, const char *except) {
    for (const unsigned char *p = (const unsigned char*)str; *p; ++p) {
        if (!isalpha(*p) && strchr(except, *p) == NULL) {
            return 1; // tìm thấy ký tự đặc biệt
        }
    }
    return 0;
}

char *capitalize(const char *str_in) {
    // convert all to lowercase, split by continuous spaces, capitalize first letter of each word, join with single space, trim trailing spaces
    size_t n = strlen(str_in);
    char *tmp = dupstr(str_in);
    for (size_t i = 0; i < n; ++i) tmp[i] = (char)tolower((unsigned char)tmp[i]);

    char *out = (char*)malloc(n + 1); // result won't be longer than input
    if (!out) exit(1);
    out[0] = '\0';

    size_t i = 0, outlen = 0;
    while (tmp[i]) {
        // skip spaces
        while (isspace((unsigned char)tmp[i])) i++;
        if (!tmp[i]) break;

        // add a space between words if needed
        if (outlen > 0) out[outlen++] = ' ';

        // process one word
        if (isalpha((unsigned char)tmp[i])) {
            out[outlen++] = (char)toupper((unsigned char)tmp[i++]);
        } else {
            out[outlen++] = tmp[i++]; // if the first character is not a letter, keep it (rare case)
        }
        while (tmp[i] && !isspace((unsigned char)tmp[i])) {
            out[outlen++] = tmp[i++];
        }
    }
    out[outlen] = '\0';
    free(tmp);
    return out;
}

// String center(String s, int width)
char *center(const char *s_in, int width) {
    const char *s = s_in ? s_in : "";
    int len = (int)strlen(s);
    if (width <= len) return dupstr(s);

    int pad = (width - len) / 2;
    char *res = (char*)malloc((size_t)width + 1);
    if (!res) exit(1);

    memset(res, ' ', width);
    memcpy(res + pad, s, (size_t)len);
    res[width] = '\0';
    return res;
}

/* ===================== Internal Utilities (for StudentService) ===================== */
int str_icmp(const char *a, const char *b) {       
#ifdef _WIN32
    return _stricmp(a, b);
#else
    while (*a && *b) {
        unsigned char ca = (unsigned char)tolower((unsigned char)*a++);
        unsigned char cb = (unsigned char)tolower((unsigned char)*b++);
        if (ca != cb) return (int)ca - (int)cb;
    }
    return (int)(unsigned char)tolower((unsigned char)*a) - (int)(unsigned char)tolower((unsigned char)*b);
#endif
}
int str_ieq(const char *a, const char *b) { return str_icmp(a, b) == 0; }
int contains_ignore_case(const char *hay, const char *needle) {
    if (!*needle) return 1;
    size_t n = strlen(needle);
    for (size_t i = 0, H = strlen(hay); i + n <= H; ++i) {
        size_t j = 0;
        for (; j < n; ++j) {
            unsigned char c1 = (unsigned char)tolower((unsigned char)hay[i + j]);
            unsigned char c2 = (unsigned char)tolower((unsigned char)needle[j]);
            if (c1 != c2) break;
        }
        if (j == n) return 1;
    }
    return 0;
}
void print_repeat(char ch, int count) {
    for (int i = 0; i < count; ++i) putchar(ch);
    putchar('\n');
}
void removeAt(ArrayList *list, int index) {
    if (index < 0 || index >= list->size) return;
    if (index < list->size - 1)
        memmove(&list->data[index], &list->data[index + 1], (size_t)(list->size - index - 1) * sizeof(Student));
    list->size--;
}
void clearList(ArrayList *list) {
    list->size = 0; 
}
ArrayList copyList(const ArrayList *src) {
    ArrayList dst;
    dst.size = src->size;
    dst.capacity = src->size > 0 ? src->size : 1;
    dst.data = (Student*)malloc((size_t)dst.capacity * sizeof(Student));
    if (!dst.data) { perror("malloc"); exit(1); }
    if (src->size > 0)
        memcpy(dst.data, src->data, (size_t)src->size * sizeof(Student));
    return dst;
}

/* helpers for "last name" (extract last word from Name) */
void last_name(const char *full, char *out, size_t out_sz) {
    out[0] = '\0';
    size_t len = strlen(full);
    if (len == 0) return;
    /* trim spaces */
    char tmp[256];
    strncpy(tmp, full, sizeof(tmp)-1); tmp[sizeof(tmp)-1] = '\0';
    trim_inplace(tmp);
    if (tmp[0] == '\0') return;
    /* find last space */
    const char *p = strrchr(tmp, ' ');
    const char *last = p ? p + 1 : tmp;
    strncpy(out, last, out_sz - 1);
    out[out_sz - 1] = '\0';
}

/* ===================== qsort Comparators ===================== */
int cmp_id_asc (const void *a, const void *b){ const Student *x=a, *y=b; return str_icmp(x->ID,    y->ID); }int cmp_id_desc(const void *a, const void *b){ return -cmp_id_asc(a,b); }
int cmp_class_asc (const void *a, const void *b){ const Student *x=a, *y=b; return str_icmp(x->Class, y->Class); }int cmp_class_desc(const void *a, const void *b){ return -cmp_class_asc(a,b); }
int cmp_gender_asc (const void *a, const void *b){ const Student *x=a, *y=b; return str_icmp(x->gender,y->gender); }int cmp_gender_desc(const void *a, const void *b){ return -cmp_gender_asc(a,b); }
int cmp_email_asc (const void *a, const void *b){ const Student *x=a, *y=b; return str_icmp(x->email, y->email); }int cmp_email_desc(const void *a, const void *b){ return -cmp_email_asc(a,b); }

int cmp_name_asc (const void *a, const void *b){
    const Student *x=a, *y=b; char lx[128], ly[128];
    last_name(x->Name, lx, sizeof(lx)); last_name(y->Name, ly, sizeof(ly));
    int t = str_icmp(lx, ly);
    if (t == 0) return str_icmp(x->Name, y->Name);
    return t;
}int cmp_name_desc(const void *a, const void *b){
    return -cmp_name_asc(a,b);
}

int cmp_dob_asc (const void *a, const void *b){ const Student *x=a, *y=b; return strcmp(x->DOB, y->DOB); }int cmp_dob_desc(const void *a, const void *b){ return -cmp_dob_asc(a,b); }

int cmp_score_asc (const void *a, const void *b){
    const Student *x=a, *y=b;
    if (x->Score < y->Score) return -1;
    if (x->Score > y->Score) return  1;
    return 0;
}int cmp_score_desc(const void *a, const void *b){ return -cmp_score_asc(a,b); }

/* ===================== Internal utilities of FileService ===================== */

int file_exists(const char *path) {
    FILE *fp = fopen(path, "r");
    if (fp) { fclose(fp); return 1; }
    return 0;
}

int ask_yes_no(const char *prompt) {
    char ans[16];
    printf("%s", prompt);
    read_line(ans, sizeof(ans));
    return (ans[0]=='y' || ans[0]=='Y');
}

void fprint_repeat(FILE *fp, char ch, int count) {
    for (int i = 0; i < count; ++i) fputc(ch, fp);
    fputc('\n', fp);
}

void compute_max_widths(const ArrayList *s, int *nameW, int *emailW) {
    int nW = *nameW, eW = *emailW;
    for (int i = 0; i < s->size; ++i) {
        int ln = (int)strlen(s->data[i].Name);
        int le = (int)strlen(s->data[i].email);
        if (ln > nW) nW = ln;
        if (le > eW) eW = le;
    }
    *nameW = nW; *emailW = eW;
}

void sanitize_csv_field_remove_commas(const char *in, char *out, size_t outsz) {
    size_t j = 0;
    for (size_t i = 0; in[i] && j + 1 < outsz; ++i) {
        if (in[i] != ',') out[j++] = in[i];
    }
    out[j] = '\0';
}

void open_file_with_default_app(const char *path) {
#if defined(_WIN32) || defined(_WIN64)
    /* Use start via cmd (escape quotes) */
    char cmd[1024];
    snprintf(cmd, sizeof(cmd), "start \"\" \"%s\"", path);
    system(cmd);
#elif defined(__APPLE__)
    char cmd[1024];
    snprintf(cmd, sizeof(cmd), "open \"%s\"", path);
    system(cmd);
#else
    char cmd[1024];
    snprintf(cmd, sizeof(cmd), "xdg-open \"%s\" >/dev/null 2>&1 &", path);
    system(cmd);
#endif
}
// ==================================Validator Functions==================
char *ID(Student *students, size_t n_students, int option) {
    char buf[MAX_LINE];
    int check;
    do {
        check = 0;
        printf("Enter ID: \n");
        read_line(buf, sizeof(buf));
        trim_inplace(buf);

        if (hasSpecialChar(buf, "0123456789")) {
            check = 1;
        }
        if ((int)strlen(buf) > 10) {
            printf("ID is too long! Please enter a shorter one.\n");
            check = 1;
        }
        if (option == 1 && !check && id_exists(students, n_students, buf)) {
            printf("ID %s already exists in the system!\n", buf);
            check = 1;
        }void trim_inplace(char *s);
    } while (check);

    char *ans = dupstr(buf);
    to_upper_inplace(ans);
    return ans;
}

char *Name(void) {
    char buf[MAX_LINE];
    int check;
    do {
        check = 0;
        printf("Enter Name: \n");
        read_line(buf, sizeof(buf));
        trim_inplace(buf);

        if ((int)strlen(buf) > 50) {
            printf("Name is too long! Please enter a shorter one.\n");
            check = 1;
        }
        if (hasSpecialChar(buf, " ")) {
            check = 1;
            printf("Name cannot contain special characters or numbers!\n\n");
        }
    } while (check);

    char *cap = capitalize(buf);
    return cap;
}

char *Class(void) {
    char buf[MAX_LINE];
    int check;
    do {
        check = 0;
        printf("Enter Class: \n");
        read_line(buf, sizeof(buf)); 

        if (hasSpecialChar(buf, "0123456789")) {
            check = 1;
            printf("ID cannot contain special characters!\n\n"); 
        }
        if ((int)strlen(buf) > 8) {
            printf("Name is too long! Please enter a shorter one.\n");
            check = 1;
        }
    } while (check);

    char *ans = dupstr(buf);
    to_upper_inplace(ans);
    return ans;
}

double Score(void) {
    char buf[MAX_LINE];
    double res = 0.0;
    int check;
    do {
        check = 0;
        printf("Enter Score: \n");
        read_line(buf, sizeof(buf));

        if (!only_digits_dot_dash(buf)) {
            check = 1;
            printf("Only digits and dots are allowed!\n\n");
        }
        if ((int)strlen(buf) > 6) {
            printf("Score is too long! Please enter a shorter one.\n");
            check = 1;
        }
        if (!check) {
            char *endptr = NULL;
            res = strtod(buf, &endptr);
            if (endptr == buf || *endptr != '\0') {
                check = 1;
                printf("Only digits and dots are allowed!\n\n");
            } else if (res > 10) {
                check = 1;
                printf("Score must be less than or equal to 10!\n");
            } else if (res < 0) {
                check = 1;
                printf("Score cannot be negative!\n");
            }
        }
    } while (check);
    return res;
}

	char *DOB(void) {
    char dob[MAX_LINE];
    int check;
    char dd[8] = "", mm[8] = "", yyyy[12] = "";
    // Lấy năm hiện tại
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int current_year = tm.tm_year + 1900;

    do {
        check = 0;
        printf("Enter Date of Birth (dd/mm/yyyy): ");
        read_line(dob, sizeof(dob));
        trim_inplace(dob);

        if (!only_digits_slash(dob)) {
            printf("Date of Birth can only contain digits and '/'\n");
            check = 1;
            continue;
        }

        // split dd/mm/yyyy
        char tmp[MAX_LINE];
        strncpy(tmp, dob, sizeof(tmp));
        tmp[sizeof(tmp)-1] = '\0';

        char *p1 = strtok(tmp, "/");
        char *p2 = strtok(NULL, "/");
        char *p3 = strtok(NULL, "/");
        char *p4 = strtok(NULL, "/");

        if (!p1 || !p2 || !p3 || p4) {
            printf("Date of Birth must be in format dd/mm/yyyy!\n");
            check = 1;
            continue;
        }

        strncpy(dd, p1, sizeof(dd)-1); dd[sizeof(dd)-1] = '\0';
        strncpy(mm, p2, sizeof(mm)-1); mm[sizeof(mm)-1] = '\0';
        strncpy(yyyy, p3, sizeof(yyyy)-1); yyyy[sizeof(yyyy)-1] = '\0';

        if ((int)strlen(dob) > 10) {
            printf("Date of Birth must be exactly 10 characters (dd/mm/yyyy).\n");
            check = 1;
            continue;
        }

        int day = atoi(dd);
        int month = atoi(mm);
        int year = atoi(yyyy);

        if (month < 1 || month > 12) {
            printf("Invalid month!\n");
            check = 1;
        } else if (year < 0) {
            printf("Invalid year!\n");
            check = 1;            
        } else if (year > current_year) {
            printf("Year cannot be greater than current year (%d)!\n", current_year);
            check = 1;
        }
         else if (year < current_year-150) {
            printf("Year must be >= %d (too old to be realistic!)\n", current_year - 150);
            check = 1;
        }
		else if ((month == 1 || month == 3 || month == 5 || month == 7 ||
                    month == 8 || month == 10 || month == 12)
                   && !(day > 0 && day <= 31)) {
            printf("Invalid day!\n");
            check = 1;
        } else if ((month == 4 || month == 6 || month == 9 || month == 11)
                   && !(day > 0 && day <= 30)) {
            printf("Invalid day!\n");
            check = 1;
        } else if (month == 2) {
            int leap = (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
            if (leap && !(day > 0 && day <= 29)) {
                printf("Invalid day for leap year!\n");
                check = 1;
            } else if (!leap && !(day > 0 && day <= 28)) {
                printf("Invalid day for non-leap year!\n");
                check = 1;
            }
        }
    } while (check);

    // pad dd, mm to 2 characters
    if (strlen(mm) == 1) { char t[3] = "0"; strncat(t, mm, 1); strcpy(mm, t); }
    if (strlen(dd) == 1) { char t[3] = "0"; strncat(t, dd, 1); strcpy(dd, t); }

    char *res = (char*)malloc(11); // dd/mm/yyyy + '\0'
    if (!res) exit(1);
    snprintf(res, 11, "%02d/%02d/%s", atoi(dd), atoi(mm), yyyy);
    return res;
}

int email_exists(const Student *arr, size_t n, const char *email) {
    for (size_t i = 0; i < n; ++i) {
        if (equals_ignore_case(arr[i].email, email)) return 1;
    }
    return 0;
}
char *email(Student *students, size_t n_students, int option) {
    char buf[MAX_LINE];
    int check;
    do {
        check = 0;
        printf("Enter email: ");
        read_line(buf, sizeof(buf));
        trim_inplace(buf);

        if (buf[0] == '\0') {
            printf("Email cannot be empty!\n");
            check = 1;
            continue;
        }

        char *at = strchr(buf, '@');
        if (!at) { printf("Invalid email format! Example: example@gmail.com\n"); check = 1; continue; }

        char *dot = strrchr(buf, '.');
        if (!dot || dot < at) { printf("Invalid email format! Example: example@gmail.com\n"); check = 1; continue; }

        // only allow 1 '@'
        if (strchr(at + 1, '@')) { printf("Invalid email format! Example: example@gmail.com\n"); check = 1; continue; }

        // local part
        *at = '\0';
        const char *local = buf;
        const char *domain = at + 1;
        if (*local == '\0') { printf("Invalid email format! Example: example@gmail.com\n"); check = 1; *at='@'; continue; }
        for (const unsigned char *p = (const unsigned char*)local; *p; ++p) {
            if (!isalnum(*p)) { printf("Invalid email format! Example: example@gmail.com\n"); check = 1; break; }
        }
        *at = '@';
        if (check) continue;

        char dom_part[MAX_LINE];
        strncpy(dom_part, domain, sizeof(dom_part)-1); 
        dom_part[sizeof(dom_part)-1] = '\0';

        char *dot2 = strrchr(dom_part, '.');
        if (!dot2) { printf("Invalid email format! Example: example@gmail.com\n"); check = 1; continue; }
        *dot2 = '\0';
        const char *dom = dom_part;
        const char *tld = dot2 + 1;

        if (*dom == '\0' || *tld == '\0') { 
            printf("Invalid email format! Example: example@gmail.com\n"); 
            check = 1; 
            continue; 
        }

        // ✅ CHỈ THÊM PHẦN NÀY — cho phép domain có nhiều dấu chấm (vd: fpt.edu.vn)
        for (const unsigned char *p = (const unsigned char*)dom; *p; ++p) {
            if (!((*p >= 'a' && *p <= 'z') || *p == '.')) {
                printf("Invalid email format! Example: example@gmail.com\n");
                check = 1;
                break;
            }
        }
        if (check) continue;

        if (dom[0] == '.' || dom[strlen(dom)-1] == '.' || strstr(dom, "..")) {
            printf("Invalid email format! Example: example@gmail.com\n");
            check = 1;
            continue;
        }
        // ✅ Hết phần thêm

        size_t tlen = strlen(tld);
        if (tlen < 2 || tlen > 6) { 
            printf("Invalid email format! Example: example@gmail.com\n"); 
            check = 1; 
            continue; 
        }
        for (const unsigned char *p = (const unsigned char*)tld; *p; ++p) {
            if (!isalpha(*p)) { 
                printf("Invalid email format! Example: example@gmail.com\n"); 
                check = 1; 
                break; 
            }
        }
        if (option == 1 && !check && email_exists(students, n_students, buf)) {
            printf("Email (%s) already exists in the system!\n", buf);
            check = 1;
        }

    } while (check);

    return dupstr(buf);
}


char *gender(void){
    char buf[MAX_LINE];
    int check;
    do {
        check = 0;
        printf("Enter gender (Male/Female/Other): ");
        read_line(buf, sizeof(buf));
        trim_inplace(buf);

        // lowercase
        for (char *p = buf; *p; ++p) *p = (char)tolower((unsigned char)*p);

        if (strcmp(buf, "male") == 0 || strcmp(buf, "m") == 0) {
            return dupstr("Male");
        } else if (strcmp(buf, "female") == 0 || strcmp(buf, "f") == 0) {
            return dupstr("Female");
        } else if (strcmp(buf, "other") == 0 || strcmp(buf, "o") == 0) {
            return dupstr("Other");
        } else {
            printf("Invalid gender! Please enter Male, Female or Other.\n");
            check = 1;
        }
    } while (check);

    return dupstr("Other");
}

//================ StudentService Functions =========================================
#include <string.h>
#include <stdio.h>

Student addStudent(ArrayList *students) {
    Student s;
    char *tmp;

    // ID
    tmp = ID(students->data, (size_t)students->size, 1);
    strncpy(s.ID, tmp, sizeof(s.ID)-1);
    s.ID[sizeof(s.ID)-1] = '\0';
    free(tmp);
    snprintf(description, sizeof(description), "Add student with ID=%s", s.ID);

    // Name
    tmp = Name();
    strncpy(s.Name, tmp, sizeof(s.Name)-1);
    s.Name[sizeof(s.Name)-1] = '\0';
    free(tmp);

    // Gender
    tmp = gender();
    strncpy(s.gender, tmp, sizeof(s.gender)-1);
    s.gender[sizeof(s.gender)-1] = '\0';
    free(tmp);

    // Class
    tmp = Class();
    strncpy(s.Class, tmp, sizeof(s.Class)-1);
    s.Class[sizeof(s.Class)-1] = '\0';
    free(tmp);

    // DOB
    tmp = DOB();
    strncpy(s.DOB, tmp, sizeof(s.DOB)-1);
    s.DOB[sizeof(s.DOB)-1] = '\0';
    free(tmp);

    // Email
    tmp = email(students->data, (size_t)students->size, 1);
    strncpy(s.email, tmp, sizeof(s.email)-1);
    s.email[sizeof(s.email)-1] = '\0';
    free(tmp);

    // Score
    s.Score = Score();

    // LOG
    OperationHistory_log("Add student", "Student", description, "OK");
    return s;
}

void displayStudents(const char *option, const char *data, const char *comparisonOperator, const ArrayList *s) {
    int maxLengthOfNames = 8;
    int maxLengthOfEmails = 12;

    for (int i = 0; i < s->size; ++i) {
        int ln = (int)strlen(s->data[i].Name);
        int le = (int)strlen(s->data[i].email);
        if (ln > maxLengthOfNames)  maxLengthOfNames  = ln;
        if (le > maxLengthOfEmails) maxLengthOfEmails = le;
    }
    int totalWidth = 64 + maxLengthOfNames + maxLengthOfEmails;

    /* top bar */
    print_repeat('=', totalWidth);

    /* header line */
    char *cNo = center("No.", 5);
    char *cID = center("ID", 10);
    char *cName = center("Name", maxLengthOfNames);
    char *cGender = center("Gender", 8);
    char *cClass = center("Class", 10);
    char *cDOB = center("Date of Birth", 14);
    char *cEmail = center("Email", maxLengthOfEmails);
    char *cScore = center("Score", 8);

    printf("|%s|%s|%s|%s|%s|%s|%s|%s|\n",
           cNo, cID, cName, cGender, cClass, cDOB, cEmail, cScore);

    free(cNo); free(cID); free(cName); free(cGender); free(cClass); free(cDOB); free(cEmail); free(cScore);

    /* mid bar */
    print_repeat('-', totalWidth);

    int cnt = 0;
    for (int i = 0; i < s->size; ++i) {
        const Student *st = &s->data[i];
        int shouldDisplay = 0;

        if (strcmp(option, "all") == 0 && (data == NULL || *data == '\0')) {
            shouldDisplay = 1;
        }
        else if (strcmp(option, "ID") == 0) {
            if (strcmp(comparisonOperator, "contains") == 0)
                shouldDisplay = contains_ignore_case(st->ID, data);
            else
                shouldDisplay = str_ieq(st->ID, data);
        }
        else if (strcmp(option, "Name") == 0) {
            if (strcmp(comparisonOperator, "contains") == 0)
                shouldDisplay = contains_ignore_case(st->Name, data);
            else
                shouldDisplay = str_ieq(st->Name, data);
        }
        else if (strcmp(option, "Gender") == 0 && str_ieq(st->gender, data)) {
            shouldDisplay = 1;
        }
        else if (strcmp(option, "Class") == 0) {
            if (strcmp(comparisonOperator, "contains") == 0)
                shouldDisplay = contains_ignore_case(st->Class, data);
            else
                shouldDisplay = str_ieq(st->Class, data);
        }
        else if (strcmp(option, "DOB") == 0 && strcmp(st->DOB, data) == 0) {
            shouldDisplay = 1;
        }
        else if (strcmp(option, "Email") == 0) {
            if (strcmp(comparisonOperator, "contains") == 0)
                shouldDisplay = contains_ignore_case(st->email, data);
            else
                shouldDisplay = str_ieq(st->email, data);
        }
        else if (strcmp(option, "Score") == 0) {
            char *endp = NULL;
            double doubleData = strtod(data ? data : "", &endp);
            if (endp && (*endp == '\0')) {
                if      (strcmp(comparisonOperator, "=" ) == 0) shouldDisplay = (st->Score == doubleData);
                else if (strcmp(comparisonOperator, "<" ) == 0) shouldDisplay = (st->Score <  doubleData);
                else if (strcmp(comparisonOperator, ">" ) == 0) shouldDisplay = (st->Score >  doubleData);
                else if (strcmp(comparisonOperator, "<=") == 0) shouldDisplay = (st->Score <= doubleData);
                else if (strcmp(comparisonOperator, ">=") == 0) shouldDisplay = (st->Score >= doubleData);
            }
        }

        if (shouldDisplay) {
            ++cnt;
            char numbuf[32]; snprintf(numbuf, sizeof(numbuf), "%d", cnt);
            char scbuf[32];  snprintf(scbuf, sizeof(scbuf),  "%.2f", st->Score);

            char *cNo2 = center(numbuf, 5);
            char *cID2 = center(st->ID, 10);
            char *cGender2 = center(st->gender, 8);
            char *cClass2  = center(st->Class, 10);
            char *cDOB2    = center(st->DOB, 14);
            char *cScore2  = center(scbuf, 8);

            printf("|%s|%s|%-*s|%s|%s|%s|%-*s|%s|\n",
                   cNo2, cID2,
                   maxLengthOfNames,  st->Name,
                   cGender2,
                   cClass2,
                   cDOB2,
                   maxLengthOfEmails, st->email,
                   cScore2);

            free(cNo2); free(cID2); free(cGender2); free(cClass2); free(cDOB2); free(cScore2);
        }
    }

    if (cnt == 0) {
        printf("No matching students found!\n");
        OperationHistory_log("Display student", "Student", "No matching students found", "ERROR");
    } else {
        OperationHistory_log("Display student", "Student", "Displayed matching students successfully", "OK");
    }

    /* bottom bar */
    print_repeat('=', totalWidth);
}


void search(ArrayList *s) {
    printf("\n=== SEARCH STUDENT MENU ===\n");
    printf("1. ID\n");
    printf("2. Name\n");
    printf("3. Class\n");
    printf("4. Score\n");
    printf("5. Date of Birth\n");
    printf("6. Gender\n");
    printf("7. Email\n");
    printf("0. Return to main menu\n");
    printf("Enter your choice: ");

    char choice[32]; 
    read_line(choice, sizeof(choice));
    char data[256]; 
    strcpy(data, "Search with ");
    int success = 0; // ✅ flag để kiểm tra có chạy thành công không

    if (strcmp(choice, "1") == 0) {
        char *id = ID(s->data, (size_t)s->size, 0);
        snprintf(data, sizeof(data), "Search by ID: %s", id);
        displayStudents("ID", id, "contains", s);
        success = 1;
        free(id);

    } else if (strcmp(choice, "2") == 0) {
        char *nm = Name();
        snprintf(data, sizeof(data), "Search by Name: %s", nm);
        displayStudents("Name", nm, "contains", s);
        success = 1;
        free(nm);

    } else if (strcmp(choice, "3") == 0) {
        char *cl = Class();
        snprintf(data, sizeof(data), "Search by Class: %s", cl);
        displayStudents("Class", cl, "contains", s);
        success = 1;
        free(cl);

    } else if (strcmp(choice, "4") == 0) {
        char option[32];
        printf("\n=== SEARCH BY SCORE ===\n");
        printf("1. Score >\n2. Score <\n3. Score <=\n4. Score >=\n5. Score =\n0. Return\n");
        printf("Enter your choice: ");
        read_line(option, sizeof(option));

        if (strcmp(option, "0") == 0) { 
            printf("Returning to main menu...\n");
            OperationHistory_log("Search student", "Student", "Return to menu", "OK");
            return;
        }

        double sc = Score();
        char buf[32]; snprintf(buf, sizeof(buf), "%.4f", sc);

        const char *op = NULL;
        if      (strcmp(option, "1")==0) op = ">";
        else if (strcmp(option, "2")==0) op = "<";
        else if (strcmp(option, "3")==0) op = "<=";
        else if (strcmp(option, "4")==0) op = ">=";
        else if (strcmp(option, "5")==0) op = "=";
        else {
            printf("Invalid choice!\n");
            OperationHistory_log("Search student", "Student", "Invalid score search option", "ERROR");
            return;
        }

        snprintf(data, sizeof(data), "Search by Score %s %.2f", op, sc);
        displayStudents("Score", buf, op, s);
        success = 1;

    } else if (strcmp(choice, "5") == 0) {
        char *d = DOB();
        snprintf(data, sizeof(data), "Search by Date of Birth: %s", d);
        displayStudents("DOB", d, "=", s);
        success = 1;
        free(d);

    } else if (strcmp(choice, "6") == 0) {
        char *g = gender();
        snprintf(data, sizeof(data), "Search by Gender: %s", g);
        displayStudents("Gender", g, "=", s);
        success = 1;
        free(g);

    } else if (strcmp(choice, "7") == 0) {
        char *em = email(s->data, (size_t)s->size, 0);
        snprintf(data, sizeof(data), "Search by Email: %s", em);
        displayStudents("Email", em, "contains", s);
        success = 1;
        free(em);

    } else if (strcmp(choice, "0") == 0) {
        printf("Returning to main menu...\n");
        OperationHistory_log("Search student", "Student", "Return to menu", "OK");
        return;

    } else {
        printf("Invalid choice!\n");
        OperationHistory_log("Search student", "Student", "Invalid choice", "ERROR");
        return;
    }

    // ✅ Ghi log chính xác theo kết quả
    if (success)
        OperationHistory_log("Search student", "Student", data, "OK");
    else
        OperationHistory_log("Search student", "Student", data, "ERROR");
}

// Tạo danh sách các index student match với điều kiện tìm kiếm,
// theo đúng logic giống displayStudents(...) đang dùng.
static int build_match_list(
    const ArrayList *s,
    const char *option,               // "ID", "Name", "Class", ...
    const char *data,                 // keyword / chuỗi so sánh / điểm dạng string
    const char *comparisonOperator,   // "contains", "=", ">", "<", "<=", ">="
    int out_idx[],                    // output: danh sách index thực trong s->data
    int max_out                       // kích thước tối đa của out_idx
) {
    int cnt = 0;

    for (int i = 0; i < s->size; ++i) {
        const Student *st = &s->data[i];
        int shouldDisplay = 0;

        if (strcmp(option, "ID") == 0) {
            if (strcmp(comparisonOperator, "contains") == 0)
                shouldDisplay = contains_ignore_case(st->ID, data);
            else
                shouldDisplay = str_ieq(st->ID, data);

        } else if (strcmp(option, "Name") == 0) {
            if (strcmp(comparisonOperator, "contains") == 0)
                shouldDisplay = contains_ignore_case(st->Name, data);
            else
                shouldDisplay = str_ieq(st->Name, data);

        } else if (strcmp(option, "Gender") == 0) {
            // displayStudents() hiện dùng so sánh đúng/đủ cho Gender (str_ieq),
            // không hỗ trợ "contains". Vì vậy ở đây ta cứ dùng str_ieq.
            shouldDisplay = str_ieq(st->gender, data);

        } else if (strcmp(option, "Class") == 0) {
            if (strcmp(comparisonOperator, "contains") == 0)
                shouldDisplay = contains_ignore_case(st->Class, data);
            else
                shouldDisplay = str_ieq(st->Class, data);

        } else if (strcmp(option, "DOB") == 0) {
            // displayStudents() dùng strcmp exact cho DOB
            shouldDisplay = (strcmp(st->DOB, data) == 0);

        } else if (strcmp(option, "Email") == 0) {
            if (strcmp(comparisonOperator, "contains") == 0)
                shouldDisplay = contains_ignore_case(st->email, data);
            else
                shouldDisplay = str_ieq(st->email, data);

        } else if (strcmp(option, "Score") == 0) {
            // giống logic displayStudents() cho Score
            char *endp = NULL;
            double doubleData = strtod(data ? data : "", &endp);
            if (endp && (*endp == '\0')) {
                if      (strcmp(comparisonOperator, "=" ) == 0) shouldDisplay = (st->Score == doubleData);
                else if (strcmp(comparisonOperator, "<" ) == 0) shouldDisplay = (st->Score <  doubleData);
                else if (strcmp(comparisonOperator, ">" ) == 0) shouldDisplay = (st->Score >  doubleData);
                else if (strcmp(comparisonOperator, "<=") == 0) shouldDisplay = (st->Score <= doubleData);
                else if (strcmp(comparisonOperator, ">=") == 0) shouldDisplay = (st->Score >= doubleData);
            }
        }

        if (shouldDisplay) {
            if (cnt < max_out) {
                out_idx[cnt] = i; // lưu index thực trong s->data
            }
            cnt++;
        }
    }

    return cnt; // số lượng match
}

void delete(ArrayList *s) {
    printf("\n=== DELETE STUDENT MENU ===\n");
    printf("1. ID\n");
    printf("2. Name\n");
    printf("3. Class\n");
    printf("4. Score\n");
    printf("5. Date of Birth\n");
    printf("6. Gender\n");
    printf("7. Email\n");
    printf("8. All data\n");
    printf("0. Exit\n");
    printf("Enter your choice: ");

    char choice[32];
    read_line(choice, sizeof(choice));
    trim_inplace(choice);

    // Thoát
    if (strcmp(choice, "0") == 0) {
        printf("Exitting to Menu...\n");
        return;
    }

    // Xóa toàn bộ giữ nguyên hành vi cũ
    if (strcmp(choice, "8") == 0) {
        clearList(s);
        printf("All student data deleted successfully!\n");
        OperationHistory_log("Delete student", "Student",
                             "Delete all student data", "OK");
        return;
    }

    // Các biến dùng chung
    char keyword[128];
    keyword[0] = '\0';

    char desc[256];
    int found_idx[1024];  // ánh xạ No. -> index thật trong s->data
    int found_count = 0;

    /***************
     * CASE 1,2,3,5,6,7: so sánh chuỗi
     ***************/
    if (strcmp(choice, "1") == 0 ||  // ID
        strcmp(choice, "2") == 0 ||  // Name
        strcmp(choice, "3") == 0 ||  // Class
        strcmp(choice, "5") == 0 ||  // DOB
        strcmp(choice, "6") == 0 ||  // Gender
        strcmp(choice, "7") == 0) {  // Email

        const char *optionField =
            (strcmp(choice, "1")==0) ? "ID" :
            (strcmp(choice, "2")==0) ? "Name" :
            (strcmp(choice, "3")==0) ? "Class" :
            (strcmp(choice, "5")==0) ? "DOB" :
            (strcmp(choice, "6")==0) ? "Gender" :
                                       "Email";

        // Chọn comparisonOperator phù hợp với displayStudents()
        // - với ID/Name/Class/Email => "contains" để hỗ trợ gõ 1 phần ("th")
        // - với DOB/Gender displayStudents không hỗ trợ "contains", chỉ so sánh chính xác
        const char *cmpOp =
            (strcmp(choice, "5")==0) ? "=" :
            (strcmp(choice, "6")==0) ? "=" :
                                       "contains";

        printf("Enter keyword for %s (or 0 to cancel): ", optionField);
        read_line(keyword, sizeof(keyword));
        trim_inplace(keyword);

        if (strcmp(keyword, "0") == 0 || keyword[0] == '\0') {
            printf("Canceled. Returning to menu.\n");
            snprintf(desc, sizeof(desc),
                     "Delete by %s: user canceled before listing", optionField);
            OperationHistory_log("Delete student", "Student", desc, "OK");
            return;
        }

        // Tìm match theo đúng logic displayStudents()
        found_count = build_match_list(
            s,
            optionField,
            keyword,
            cmpOp,
            found_idx,
            (int)(sizeof(found_idx)/sizeof(found_idx[0]))
        );

        if (found_count == 0) {
            printf("No matching students found!\n");
            snprintf(desc, sizeof(desc),
                     "Delete by %s: \"%s\" → No student found",
                     optionField, keyword);
            OperationHistory_log("Delete student", "Student", desc, "ERROR");
            return;
        }

        // In danh sách bằng displayStudents() cho user xem
        // Bảng sẽ đánh No. = 1..found_count theo đúng thứ tự duyệt
        displayStudents(optionField, keyword, cmpOp, s);

        // Hỏi người dùng muốn xóa mục số mấy
        printf("Enter the number (1-%d) to delete, or 0 to cancel: ", found_count);

        char pickbuf[32];
        read_line(pickbuf, sizeof(pickbuf));
        trim_inplace(pickbuf);

        int pick = atoi(pickbuf);
        if (pick == 0) {
            printf("Canceled. Returning to menu.\n");
            snprintf(desc, sizeof(desc),
                     "Delete by %s: \"%s\" → User canceled after listing",
                     optionField, keyword);
            OperationHistory_log("Delete student", "Student", desc, "OK");
            return;
        }
        if (pick < 1 || pick > found_count) {
            printf("Invalid selection!\n");
            snprintf(desc, sizeof(desc),
                     "Delete by %s: \"%s\" → Invalid selection index",
                     optionField, keyword);
            OperationHistory_log("Delete student", "Student", desc, "ERROR");
            return;
        }

        // Lấy index thật trong ArrayList
        int realIndex = found_idx[pick - 1];
        Student removed = s->data[realIndex]; // copy để log trước khi xóa

        // Xóa đúng 1 student
        removeAt(s, realIndex);

        printf("Deleted 1 student successfully!\n");

        snprintf(desc, sizeof(desc),
                 "Delete by %s: \"%s\" → deleted ID %s, Name %s",
                 optionField, keyword, removed.ID, removed.Name);
        OperationHistory_log("Delete student", "Student", desc, "OK");

        return;
    }

    /***************
     * CASE 4: Score
     ***************/
    if (strcmp(choice, "4") == 0) {
        char option[8];
        printf("\n=== DELETE BY SCORE ===\n");
        printf("1. Score >\n");
        printf("2. Score <\n");
        printf("3. Score <=\n");
        printf("4. Score >=\n");
        printf("5. Score =\n");
        printf("0. Return\n");
        printf("Enter your choice: ");

        read_line(option, sizeof(option));
        trim_inplace(option);

        if (strcmp(option, "0") == 0) {
            printf("Returning to main menu...\n");
            OperationHistory_log("Delete student", "Student",
                                 "Delete by Score: user canceled at condition menu", "OK");
            return;
        }

        if (!(strlen(option)==1 && option[0]>='1' && option[0]<='5')) {
            printf("Invalid choice! Please enter from 1–5.\n");
            OperationHistory_log("Delete student", "Student",
                                 "Delete by Score: invalid condition", "ERROR");
            return;
        }

        double sc = Score(); // người dùng nhập điểm tham chiếu
        char scoreStr[32];
        snprintf(scoreStr, sizeof(scoreStr), "%.4f", sc);

        const char *cmpOp =
            (strcmp(option,"1")==0) ? ">"  :
            (strcmp(option,"2")==0) ? "<"  :
            (strcmp(option,"3")==0) ? "<=" :
            (strcmp(option,"4")==0) ? ">=" : "=";

        // Tìm match theo điều kiện điểm y như displayStudents("Score", buf, cmpOp, s)
        found_count = build_match_list(
            s,
            "Score",
            scoreStr,
            cmpOp,
            found_idx,
            (int)(sizeof(found_idx)/sizeof(found_idx[0]))
        );

        if (found_count == 0) {
            printf("No matching students found!\n");
            snprintf(desc, sizeof(desc),
                     "Delete by Score %s %.2f → No student found",
                     cmpOp, sc);
            OperationHistory_log("Delete student", "Student", desc, "ERROR");
            return;
        }

        // In danh sách bằng displayStudents() cho user xem
        displayStudents("Score", scoreStr, cmpOp, s);

        // Hỏi user xóa mục nào
        printf("Enter the number (1-%d) to delete, or 0 to cancel: ", found_count);

        char pickbuf[32];
        read_line(pickbuf, sizeof(pickbuf));
        trim_inplace(pickbuf);

        int pick = atoi(pickbuf);
        if (pick == 0) {
            printf("Canceled. Returning to menu.\n");
            snprintf(desc, sizeof(desc),
                     "Delete by Score %s %.2f → User canceled after listing",
                     cmpOp, sc);
            OperationHistory_log("Delete student", "Student", desc, "OK");
            return;
        }
        if (pick < 1 || pick > found_count) {
            printf("Invalid selection!\n");
            snprintf(desc, sizeof(desc),
                     "Delete by Score %s %.2f → Invalid selection index",
                     cmpOp, sc);
            OperationHistory_log("Delete student", "Student", desc, "ERROR");
            return;
        }

        // Xóa 1 student theo lựa chọn
        int realIndex = found_idx[pick - 1];
        Student removed = s->data[realIndex];
        removeAt(s, realIndex);

        printf("Deleted 1 student successfully!\n");

        snprintf(desc, sizeof(desc),
                 "Delete by Score %s %.2f → deleted ID %s, Name %s",
                 cmpOp, sc, removed.ID, removed.Name);
        OperationHistory_log("Delete student", "Student", desc, "OK");

        return;
    }

    // Nếu tới đây mà không match case nào:
    printf("Invalid choice!\n");
    OperationHistory_log("Delete student", "Student",
                         "Invalid delete menu choice", "ERROR");
}


void update(ArrayList *s) {
    int idx = 0;
    char IDbuf[128];
    char des[256];
    strcpy(IDbuf, ID(s->data, (size_t)s->size, 0));

    while (idx < s->size) {
        if (str_ieq(s->data[idx].ID, IDbuf)) break;
        idx++;
    }

    if (idx == s->size) {
        printf("ID %s NOT FOUND!\nUpdate failed.\n", IDbuf);
        sprintf(des, "Update failed: ID %s not found", IDbuf);
        OperationHistory_log("Update student", "Student", des, "ERROR");
        return;
    }

    displayStudents("ID", IDbuf, "=", s);

    printf("\n=== UPDATE MENU ===\n");
    printf("1. ID\n");
    printf("2. Name\n");
    printf("3. Class\n");
    printf("4. Score\n");
    printf("5. Date of Birth\n");
    printf("6. Gender\n");
    printf("7. Email\n");
    printf("0. Return to menu\n");
    printf("Enter your choice: ");

    char choice[32];
    read_line(choice, sizeof(choice));

    if (strcmp(choice, "0") == 0) {
        printf("Returning to main menu...\n");
        OperationHistory_log("Update student", "Student", "User canceled update", "OK");
        return;
    }

    if (strcmp(choice, "1") == 0) {
        char *v = ID(s->data, (size_t)s->size, 1);
        sprintf(des, "Update ID: old=%s -> new=%s", s->data[idx].ID, v);
        strncpy(s->data[idx].ID, v, sizeof(s->data[idx].ID) - 1);
        s->data[idx].ID[sizeof(s->data[idx].ID) - 1] = '\0';
        OperationHistory_log("Update student", "Student", des, "OK");
        free(v);

    } else if (strcmp(choice, "2") == 0) {
        char *v = Name();
        sprintf(des, "Update Name: old=%s -> new=%s", s->data[idx].Name, v);
        strncpy(s->data[idx].Name, v, sizeof(s->data[idx].Name) - 1);
        s->data[idx].Name[sizeof(s->data[idx].Name) - 1] = '\0';
        OperationHistory_log("Update student", "Student", des, "OK");
        free(v);

    } else if (strcmp(choice, "3") == 0) {
        char *v = Class();
        sprintf(des, "Update Class: old=%s -> new=%s", s->data[idx].Class, v);
        strncpy(s->data[idx].Class, v, sizeof(s->data[idx].Class) - 1);
        s->data[idx].Class[sizeof(s->data[idx].Class) - 1] = '\0';
        OperationHistory_log("Update student", "Student", des, "OK");
        free(v);

    } else if (strcmp(choice, "4") == 0) {
        double old = s->data[idx].Score;
        s->data[idx].Score = Score();
        sprintf(des, "Update Score: old=%.2lf -> new=%.2lf", old, s->data[idx].Score);
        OperationHistory_log("Update student", "Student", des, "OK");

    } else if (strcmp(choice, "5") == 0) {
        char *v = DOB();
        sprintf(des, "Update DOB: old=%s -> new=%s", s->data[idx].DOB, v);
        strncpy(s->data[idx].DOB, v, sizeof(s->data[idx].DOB) - 1);
        s->data[idx].DOB[sizeof(s->data[idx].DOB) - 1] = '\0';
        OperationHistory_log("Update student", "Student", des, "OK");
        free(v);

    } else if (strcmp(choice, "6") == 0) {
        char *v = gender();
        sprintf(des, "Update Gender: old=%s -> new=%s", s->data[idx].gender, v);
        strncpy(s->data[idx].gender, v, sizeof(s->data[idx].gender) - 1);
        s->data[idx].gender[sizeof(s->data[idx].gender) - 1] = '\0';
        OperationHistory_log("Update student", "Student", des, "OK");
        free(v);

    } else if (strcmp(choice, "7") == 0) {
        char *v = email(s->data, (size_t)s->size, 0);
        sprintf(des, "Update Email: old=%s -> new=%s", s->data[idx].email, v);
        strncpy(s->data[idx].email, v, sizeof(s->data[idx].email) - 1);
        s->data[idx].email[sizeof(s->data[idx].email) - 1] = '\0';
        OperationHistory_log("Update student", "Student", des, "OK");
        free(v);

    } else {
        printf("Invalid choice!\n");
        OperationHistory_log("Update student", "Student", "Invalid update choice", "ERROR");
        return;
    }

    printf("Update successfully!\n");
}


void sort(ArrayList *s) {
    printf("\n=== SORT STUDENTS ===\n");
    printf("1. ID\n");
    printf("2. Name\n");
    printf("3. Class\n");
    printf("4. Score\n");
    printf("5. Date of Birth\n");
    printf("6. Gender\n");
    printf("7. Email\n");
    printf("Enter your choice: ");
    char choice[32]; read_line(choice, sizeof(choice));

    printf("\nSort order:\n1. Ascending\n2. Descending\n");
    printf("Enter your choice: ");
    char option[32]; read_line(option, sizeof(option));

    ArrayList s_copy = copyList(s);
    char des[100];
    const char *order = (strcmp(option, "1") == 0) ? "ascending" : "descending";

    if (strcmp(choice, "1") == 0) {
        qsort(s_copy.data, (size_t)s_copy.size, sizeof(Student),
              (strcmp(option,"1")==0) ? cmp_id_asc : cmp_id_desc);
        displayStudents("all", "", "", &s_copy);
        sprintf(des, "Sorted by ID in %s order", order);
        OperationHistory_log("Sort student", "Student", des, "OK");

    } else if (strcmp(choice, "2") == 0) {
        qsort(s_copy.data, (size_t)s_copy.size, sizeof(Student),
              (strcmp(option,"1")==0) ? cmp_name_asc : cmp_name_desc);
        displayStudents("all", "", "", &s_copy);
        sprintf(des, "Sorted by Name in %s order", order);
        OperationHistory_log("Sort student", "Student", des, "OK");

    } else if (strcmp(choice, "3") == 0) {
        qsort(s_copy.data, (size_t)s_copy.size, sizeof(Student),
              (strcmp(option,"1")==0) ? cmp_class_asc : cmp_class_desc);
        displayStudents("all", "", "", &s_copy);
        sprintf(des, "Sorted by Class in %s order", order);
        OperationHistory_log("Sort student", "Student", des, "OK");

    } else if (strcmp(choice, "4") == 0) {
        qsort(s_copy.data, (size_t)s_copy.size, sizeof(Student),
              (strcmp(option,"1")==0) ? cmp_score_asc : cmp_score_desc);
        displayStudents("all", "", "", &s_copy);
        sprintf(des, "Sorted by Score in %s order", order);
        OperationHistory_log("Sort student", "Student", des, "OK");

    } else if (strcmp(choice, "5") == 0) {
        qsort(s_copy.data, (size_t)s_copy.size, sizeof(Student),
              (strcmp(option,"1")==0) ? cmp_dob_asc : cmp_dob_desc);
        displayStudents("all", "", "", &s_copy);
        sprintf(des, "Sorted by Date of Birth in %s order", order);
        OperationHistory_log("Sort student", "Student", des, "OK");

    } else if (strcmp(choice, "6") == 0) {
        qsort(s_copy.data, (size_t)s_copy.size, sizeof(Student),
              (strcmp(option,"1")==0) ? cmp_gender_asc : cmp_gender_desc);
        displayStudents("all", "", "", &s_copy);
        sprintf(des, "Sorted by Gender in %s order", order);
        OperationHistory_log("Sort student", "Student", des, "OK");

    } else if (strcmp(choice, "7") == 0) {
        qsort(s_copy.data, (size_t)s_copy.size, sizeof(Student),
              (strcmp(option,"1")==0) ? cmp_email_asc : cmp_email_desc);
        displayStudents("all", "", "", &s_copy);
        sprintf(des, "Sorted by Email in %s order", order);
        OperationHistory_log("Sort student", "Student", des, "OK");

    } else {
        printf("Invalid choice!\n");
        OperationHistory_log("Sort student", "Student", "Invalid sort choice", "ERROR");
    }

    free(s_copy.data);
}


//==================FileService Functions================================
void saveFile(ArrayList *s) {
    char choice[16];
    printf("1. Save data in file .txt\n");
    printf("2. Save data in file .csv\n");
    printf("0. Exit\n");
    printf("Enter your choice: ");
    read_line(choice, sizeof(choice));

    if (strcmp(choice, "0") == 0) {
        printf("Exitting...\n");
        OperationHistory_log("Save file", "None", "User canceled save operation", "OK");
        return;
    }

    if (strcmp(choice, "1") == 0) {
        saveTextFile(s);
    } else if (strcmp(choice, "2") == 0) {
        saveCSVFile(s);
    } else {
        printf("Invalid choice!\n");
        OperationHistory_log("Save file", "None", "Invalid save choice", "ERROR");
    }
}

void loadFile(ArrayList *s) {
    char choice[16];
    printf("1. Load data in file .txt\n");
    printf("2. Load data in file .csv\n");
    printf("0. Exit\n");
    printf("Enter your choice: ");
    read_line(choice, sizeof(choice));

    if (strcmp(choice, "0") == 0) {
        printf("Exitting...\n");
        OperationHistory_log("Load file", "None", "User canceled load operation", "OK");
        return;
    }

    if (strcmp(choice, "1") == 0) {
        loadTextFile(s);
    } else if (strcmp(choice, "2") == 0) {
        loadCSVFile(s);
    } else {
        printf("Invalid choice!\n");
        OperationHistory_log("Load file", "None", "Invalid load choice", "ERROR");
    }
}

/* ------------ .txt (pretty table) ------------ */
void saveTextFile(ArrayList *s) {
    char fileName[256];
    printf("Enter file name to save (e.g. students.txt): ");
    read_line(fileName, sizeof(fileName));
    trim_inplace(fileName);

    if (fileName[0] == '\0') {
        printf("File name cannot be empty!\n");
        OperationHistory_log("Save file", "Unknown", "TXT file name empty", "ERROR");
        return;
    }

    int append = 0;
    if (file_exists(fileName)) {
        printf("File '%s' already exists.\n", fileName);
        if (ask_yes_no("Do you want to overwrite it? (y/n): ")) {
            FILE *fp = fopen(fileName, "w");
            if (fp) fclose(fp);
            printf("Old data cleared.\n");
        } else {
            append = 1;
            printf("Data will be appended to existing file.\n");
        }
    }

    int nameW=8, emailW=12;
    compute_max_widths(s, &nameW, &emailW);
    int totalWidth = 80 + nameW + emailW;

    FILE *fp = fopen(fileName, append ? "a" : "w");
    if (!fp) {
        perror("fopen");
        OperationHistory_log("Save file", fileName, "Failed to open TXT file", "ERROR");
        return;
    }

    fprint_repeat(fp, '=', totalWidth);

    char *cNo     = center("No.", 5);
    char *cID     = center("ID", 10);
    char *cName   = center("Name", nameW);
    char *cGender = center("Gender", 8);
    char *cClass  = center("Class", 10);
    char *cDOB    = center("Date of Birth", 14);
    char *cEmail  = center("Email", emailW);
    char *cScore  = center("Score", 8);

    fprintf(fp, "| %s | %s | %s | %s | %s | %s | %s | %s |\n",
            cNo, cID, cName, cGender, cClass, cDOB, cEmail, cScore);
    fprint_repeat(fp, '-', totalWidth);

    free(cNo); free(cID); free(cName); free(cGender);
    free(cClass); free(cDOB); free(cEmail); free(cScore);

    for (int i = 0; i < s->size; ++i) {
        Student *st = &s->data[i];
        char numbuf[32]; snprintf(numbuf, sizeof(numbuf), "%d", i+1);
        char scbuf[32];  snprintf(scbuf, sizeof(scbuf), "%.2f", st->Score);

        char *cNo2 = center(numbuf, 5);
        char *cID2 = center(st->ID, 10);
        char *cGender2 = center(st->gender, 8);
        char *cClass2  = center(st->Class, 10);
        char *cDOB2    = center(st->DOB, 14);
        char *cScore2  = center(scbuf, 8);

        fprintf(fp, "| %s | %s | %-*s | %s | %s | %s | %-*s | %s |\n",
                cNo2, cID2,
                nameW,  st->Name,
                cGender2,
                cClass2,
                cDOB2,
                emailW, st->email,
                cScore2);

        free(cNo2); free(cID2); free(cGender2);
        free(cClass2); free(cDOB2); free(cScore2);
    }

    fprint_repeat(fp, '=', totalWidth);
    fclose(fp);
    printf("Data saved successfully to %s\n", fileName);

    char des[256];
    sprintf(des, "Saved %d students to TXT file", s->size);
    OperationHistory_log("Save file", fileName, des, "OK");
}

static void trim(char *s){
    char *p = s;
    while (*p && isspace((unsigned char)*p)) p++;
    if (p != s) memmove(s, p, strlen(p)+1);
    char *e = s + strlen(s);
    while (e > s && isspace((unsigned char)e[-1])) *--e = '\0';
}

void loadTextFile(ArrayList *s) {
    char fileName[256];
    printf("Enter file name to load: ");
    read_line(fileName, sizeof(fileName));
    trim_inplace(fileName);

    if (!file_exists(fileName)) {
        printf("File not found!\n");
        OperationHistory_log("Load file", fileName, "Text file not found", "ERROR");
        return;
    }

    FILE *fp = fopen(fileName, "r");
    if (!fp) {
        perror("fopen");
        OperationHistory_log("Load file", fileName, "Failed to open text file", "ERROR");
        return;
    }

    char line[MAX_LINE];
    int beforeCount = s->size;

    // Skip headers
    for (int i = 0; i < 4; ++i) {
        long pos = ftell(fp);
        if (!fgets(line, sizeof(line), fp)) break;
        if (line[0] != '=' && line[0] != '-' && line[0] != '\n' && line[0] != '\r') {
            fseek(fp, pos, SEEK_SET);
            break;
        }
    }

    while (fgets(line, sizeof(line), fp)) {
        if (line[0] == '=' || line[0] == '-' || line[0] == '\n' || line[0] == '\r') continue;

        int no;
        Student stu; memset(&stu, 0, sizeof(stu));
        char id[32], name[128], gender[16], cls[32], dob[32], email[128];
        double score;

        int n = sscanf(line,
            " | %d | %31[^|] | %127[^|] | %15[^|] | %31[^|] | %31[^|] | %127[^|] | %lf |",
            &no, id, name, gender, cls, dob, email, &score);

        if (n != 8) continue;

        trim(id); trim(name); trim(gender); trim(cls); trim(dob); trim(email);

        strncpy(stu.ID, id, sizeof(stu.ID) - 1);
        strncpy(stu.Name, name, sizeof(stu.Name) - 1);
        strncpy(stu.gender, gender, sizeof(stu.gender) - 1);
        strncpy(stu.Class, cls, sizeof(stu.Class) - 1);
        strncpy(stu.DOB, dob, sizeof(stu.DOB) - 1);
        strncpy(stu.email, email, sizeof(stu.email) - 1);
        stu.Score = score;

        addStudentToList(s, stu);
    }

    fclose(fp);
    int loaded = s->size - beforeCount;
    printf("Loaded successfully! Total students: %d (added %d)\n", s->size, loaded);

    char des[256];
    if (loaded > 0)
        sprintf(des, "Loaded %d students from text file", loaded);
    else
        sprintf(des, "Loaded 0 students from text file (no valid data)");

    OperationHistory_log("Load file", fileName, des, "OK");
}




/* ------------ .csv ------------ */
void saveCSVFile(ArrayList *s) {
    char fileName[256];
    printf("Enter CSV file name to save (e.g. students.csv): ");
    read_line(fileName, sizeof(fileName));
    trim_inplace(fileName);

    if (fileName[0] == '\0') {
        printf("File name cannot be empty!\n");
        OperationHistory_log("Save file", "Unknown", "CSV file name empty", "ERROR");
        return;
    }

    if (strlen(fileName) < 4 || strcmp(fileName + strlen(fileName) - 4, ".csv") != 0) {
        strncat(fileName, ".csv", sizeof(fileName) - strlen(fileName) - 1);
    }

    int append = 0;
    if (file_exists(fileName)) {
        printf("File '%s' already exists.\n", fileName);
        if (ask_yes_no("Do you want to overwrite it? (y/n): ")) {
            FILE *fp = fopen(fileName, "w");
            if (fp) fclose(fp);
            printf(" Old data cleared.\n");
        } else {
            append = 1;
            printf(" Data will be appended to existing file.\n");
        }
    } else {
        printf("Will create: %s\n", fileName);
    }

    FILE *fp = fopen(fileName, append ? "a" : "w");
    if (!fp) {
        perror("fopen");
        OperationHistory_log("Save file", fileName, "Failed to open CSV file", "ERROR");
        return;
    }

    if (!append) {
        fprintf(fp, "No.,ID,Name,Gender,Class,Date of Birth,Email,Score\n");
    }

    for (int i = 0; i < s->size; ++i) {
        Student *st = &s->data[i];
        char name_sanit[256];
        sanitize_csv_field_remove_commas(st->Name, name_sanit, sizeof(name_sanit));
        fprintf(fp, "%d,%s,%s,%s,%s,%s,%s,%.2f\n",
                i + 1,
                st->ID,
                name_sanit,
                st->gender,
                st->Class,
                st->DOB,
                st->email,
                st->Score);
    }

    fclose(fp);
    printf("Data exported successfully to CSV file: %s\n", fileName);

    char des[256];
    sprintf(des, "Saved %d students to CSV file", s->size);
    OperationHistory_log("Save file", fileName, des, "OK");

    printf("You want to open file ? (y/n)\n");
    char choice[100];
    fgets(choice, sizeof(choice), stdin);
    choice[strcspn(choice, "\n")] = 0;
    switch (choice[0]) {
        case 'y':
            open_file_with_default_app(fileName);
            break;
        default:
            printf("Exiting...");
    }
}

void loadCSVFile(ArrayList *s) {
    char fileName[256];
    printf("Enter CSV file name to load (e.g. students.csv): ");
    read_line(fileName, sizeof(fileName));
    trim_inplace(fileName);

    if (!file_exists(fileName)) {
        printf("File not found!\n");
        OperationHistory_log("Load file", fileName, "CSV file not found", "ERROR");
        return;
    }

    FILE *fp = fopen(fileName, "r");
    if (!fp) {
        perror("fopen");
        OperationHistory_log("Load file", fileName, "Failed to open CSV file", "ERROR");
        return;
    }

    char line[1024];
    int beforeCount = s->size;

    fgets(line, sizeof(line), fp); // skip header

    while (fgets(line, sizeof(line), fp)) {
        if (line[0] == '\n' || line[0] == '\r') continue;

        Student stu;
        memset(&stu, 0, sizeof(stu));

        char *ord = strtok(line, ",");
        char *id = strtok(NULL, ",");
        char *name = strtok(NULL, ",");
        char *gender = strtok(NULL, ",");
        char *cls = strtok(NULL, ",");
        char *dob = strtok(NULL, ",");
        char *email = strtok(NULL, ",");
        char *scoreStr = strtok(NULL, ",\n\r");

        if (!ord || !id || !name || !gender || !cls || !dob || !email || !scoreStr)
            continue;

        trim_inplace(ord);
        trim_inplace(id); trim_inplace(name); trim_inplace(gender);
        trim_inplace(cls); trim_inplace(dob); trim_inplace(email); trim_inplace(scoreStr);

        strncpy(stu.ID, id, sizeof(stu.ID) - 1);
        strncpy(stu.Name, name, sizeof(stu.Name) - 1);
        strncpy(stu.gender, gender, sizeof(stu.gender) - 1);
        strncpy(stu.Class, cls, sizeof(stu.Class) - 1);
        strncpy(stu.DOB, dob, sizeof(stu.DOB) - 1);
        strncpy(stu.email, email, sizeof(stu.email) - 1);
        stu.Score = atof(scoreStr);

        addStudentToList(s, stu);
    }

    fclose(fp);
    int loaded = s->size - beforeCount;
    printf("Loaded successfully! Total students: %d (added %d)\n", s->size, loaded);

    char des[256];
    if (loaded > 0)
        sprintf(des, "Loaded %d students from CSV file", loaded);
    else
        sprintf(des, "Loaded 0 students from CSV file (no valid data)");

    OperationHistory_log("Load file", fileName, des, "OK");
}
// ================== Sub-functions for Setting ==================
void setting_history(void) {
    history();
    OperationHistory_log("Setting", "History", "Opened operation history menu", "OK");
}

void display_top10(ArrayList *s) {
    if (s->size == 0) {
        printf("No student data available!\n");
        OperationHistory_log("Setting", "Top 10", "No student data to display top list", "ERROR");
        return;
    }

    printf("\n======== TOP 10 STUDENTS (Highest Scores) ========\n");

    // Copy danh sách để không ảnh hưởng dữ liệu chính
    ArrayList copy = copyList(s);
    qsort(copy.data, (size_t)copy.size, sizeof(Student), cmp_score_desc);

    int limit = (copy.size < 10) ? copy.size : 10;

    ArrayList top10;
    top10.data = malloc(sizeof(Student) * limit);
    top10.size = limit;
    for (int i = 0; i < limit; ++i)
        top10.data[i] = copy.data[i];

    displayStudents("all", "", "", &top10);

    printf("Displayed Top %d students by score successfully!\n", limit);

    char des[128];
    sprintf(des, "Displayed Top %d students by score", limit);
    OperationHistory_log("Setting", "Top 10", des, "OK");

    free(copy.data);
    free(top10.data);
}
void statistics(ArrayList *s) {
    if (s->size == 0) {
        printf("No data!\n");
        return;
    }

	    float sum = 0, maxScore = s->data[0].Score, minScore = s->data[0].Score;
	int countExcellent = 0, countGood = 0, countAverage = 0, countWeak = 0;
	
	for (int i = 0; i < s->size; i++) {
	    float score = s->data[i].Score;
	    sum += score;
	    if (score > maxScore) maxScore = score;
	    if (score < minScore) minScore = score;
	
	    if (score >= 8) countExcellent++;
	    else if (score >= 7) countGood++;
	    else if (score >= 5) countAverage++;
	    else countWeak++;
	}
	
	printf("\n===== CLASS STATISTICS =====\n");
	printf("Total students: %d\n", s->size);
	printf("Average Score: %.2f\n", sum / s->size);
	printf("Highest Score: %.2f\n", maxScore);
	printf("Lowest Score: %.2f\n", minScore);
	printf("Excellent: %d | Good: %d | Average: %d | Weak: %d\n",
	       countExcellent, countGood, countAverage, countWeak);
	
	OperationHistory_log("Setting", "Statistics", "Displayed class statistics", "OK");

}

// ================== Setting =================================
void setting(ArrayList *s) {
    printf("======== MENU SETTING ===========\n");
    printf("1. History\n");
    printf("2. View Top 10 Highest Scores (BXH)\n");
    printf("3. Statistics\n");
    printf("0. Exit\n");
    printf("Enter your choice: ");

    char choice[100];
    fgets(choice, sizeof(choice), stdin);
    choice[strcspn(choice, "\n")] = 0;

    switch (choice[0]) {
        case '1':
            setting_history();
            break;

        case '2':
            display_top10(s);
            break;

        case '0':
            printf("Exiting...\n");
            OperationHistory_log("Setting", "Menu", "User exited setting menu", "OK");
            break;
		case '3':
			statistics(s);
        default:
            printf("Invalid choice!\n");
            OperationHistory_log("Setting", "Menu", "Invalid setting menu choice", "ERROR");
    }
}

// =================== History ================================
void history(void) {
    printf("Menu History: \n");
    printf("1. Open operationHistory in Excel (Read-Only)\n");
    printf("2. Clear all data in operation History\n");
    printf("Enter your choice:\n");

    char choice[100];
    fgets(choice, sizeof(choice), stdin);
    choice[strcspn(choice, "\n")] = 0;

    switch (choice[0]) {
        case '1': {
            // ✅ Gọi Excel ở chế độ đọc
            system("start excel.exe /r \"operationHistory.csv\"");
            printf("Opened operationHistory.csv in Excel (read-only mode).\n");
            break;
        }
        case '2': {
            OperationHistory_clear();
            break;
        }
        case '0':
            printf("Exiting...\n");
            break;
        default:
            printf("Invalid choice!\n");
    }
}


/* ========= Internal utilities (static) ========= */
static void OperationHistory_at_exit(void) {
    if (bw) {
        fflush(bw);
        fclose(bw);
        bw = NULL;
        fw = NULL;
    }
}

static void current_time_str(char *buf, size_t n) {
    time_t t = time(NULL);
    struct tm tm_info;

#if defined(_WIN32)
    #ifdef _MSC_VER
        localtime_s(&tm_info, &t);
    #else
        struct tm *p = localtime(&t);  
        if (p) tm_info = *p;
    #endif
#else
    localtime_r(&t, &tm_info);
#endif

    strftime(buf, n, "%d/%m/%Y %H:%M:%S", &tm_info);
}


static void ensure_initialized(void) {
    if (!initialized) OperationHistory_init();
}
/* ========= Public API impl ========= */
void OperationHistory_init(void) {
    if (initialized) return;

    fw = fopen(FILE_NAME, "a+"); 
    if (!fw) {
        fprintf(stderr, "Error initializing file writer\n");
        return;
    }
    bw = fw;

    if (fseek(bw, 0, SEEK_END) == 0) {
        long size = ftell(bw);
        if (size == 0) {
            fputs("No.,Time,Action,Target,Description,Status\n", bw);
            fflush(bw);
            count = 0;
        } else {
            if (fseek(bw, 0, SEEK_SET) != 0) {
                count = 0;
            } else {
                char line[512];
                int lastNo = 0;

                if (fgets(line, sizeof(line), bw) != NULL) {
                    while (fgets(line, sizeof(line), bw)) {
                        int no;
                        if (sscanf(line, "%d,", &no) == 1) {
                            lastNo = no;
                        }
                    }
                }
                count = lastNo; 
            }
            fseek(bw, 0, SEEK_END);
        }
    } else {
        count = 0;
    }

    atexit(OperationHistory_at_exit); 
    initialized = 1;
}

// Helper: loại bỏ dấu phẩy, \r\n và thay mũi tên UTF-8 bằng "->"
static void sanitize(char *dst, size_t dstsz, const char *src) {
    size_t j = 0;
    for (size_t i = 0; src && src[i] && j < dstsz - 1; ++i) {
        unsigned char c = (unsigned char)src[i];

        // UTF-8 for '→' is E2 86 92
        if (c == 0xE2 && (unsigned char)src[i+1] == 0x86 && (unsigned char)src[i+2] == 0x92) {
            if (j < dstsz - 1) dst[j++] = '-';
            if (j < dstsz - 1) dst[j++] = '>';
            i += 2; // skip the rest of the sequence
            continue;
        }

        if (c == ',' || c == '\r' || c == '\n') {
            dst[j++] = ' ';              // thay bằng space
        } else if (c < 32 || c == 127) { // ký tự điều khiển
            dst[j++] = ' ';
        } else {
            dst[j++] = (char)c;
        }
    }
    dst[j] = '\0';
}

void OperationHistory_log(const char *action,
                          const char *target,
                          const char *desc,
                          const char *status) {
    FILE *f = fopen(FILE_NAME, "a");
    if (!f) { printf("Cannot write log: file may be open in another program.\n"); return; }

    count++;
    char timebuf[32]; current_time_str(timebuf, sizeof timebuf);

    char a[256], t[256], d[1024], s[128];
    sanitize(a, sizeof a, action);
    sanitize(t, sizeof t, target);
    sanitize(d, sizeof d, desc);
    sanitize(s, sizeof s, status);

    fprintf(f, "%d,%s,%s,%s,%s,%s\n", count, timebuf, a, t, d, s);
    fclose(f);
}


void OperationHistory_close(void) {
    OperationHistory_at_exit();
    initialized = 0;
}

void OperationHistory_clear(void) {
    FILE *f = fopen(FILE_NAME, "w");
    if (!f) {
        fprintf(stderr, "Error clearing log file: cannot open %s\n", FILE_NAME);
        return;
    }

    fputs("No.,Time,Action,Target,Description,Status\n", f);
    fclose(f);

    count = 0;
    printf("Cleared all operation logs (header kept)\n");
}
