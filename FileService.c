
void saveTextFile(Student list[], int n);
void saveCSVFile(Student list[], int n);
void loadTextFile(Student list[], int *n);
void loadCSVFile(Student list[], int *n);

// Hàm lưu file
void saveFile(Student list[], int n) {
    int choice;
    printf("\n1. Save data in file .txt");
    printf("\n2. Save data in file .csv");
    printf("\n0. Exit");
    printf("\nEnter your choice: ");
    scanf("%d", &choice);
    getchar(); // đọc bỏ ký tự Enter

    switch (choice) {
        case 1:
            saveTextFile(list, n);
            break;
        case 2:
            saveCSVFile(list, n);
            break;
        case 0:
            printf("Exiting...\n");
            break;
        default:
            printf("Invalid choice!\n");
    }
}

// Hàm đọc file
void loadFile(Student list[], int *n) {
    int choice;
    printf("\n1. Load data in file .txt");
    printf("\n2. Load data in file .csv");
    printf("\n0. Exit");
    printf("\nEnter your choice: ");
    scanf("%d", &choice);
    getchar(); // đọc bỏ ký tự Enter

    switch (choice) {
        case 1:
            loadTextFile(list, n);
            break;
        case 2:
            loadCSVFile(list, n);
            break;
        case 0:
            printf("Exiting...\n");
            break;
        default:
            printf("Invalid choice!\n");
    }
}

void saveTextFile(Student list[], int n) {
    char fileName[100];
    char choice;
    FILE *f;

    printf("Enter file name to save (e.g. students.txt): ");
    scanf("%s", fileName);
    getchar(); // bỏ Enter

    // Kiểm tra file có tồn tại không
    f = fopen(fileName, "r");
    bool append = false;

    if (f != NULL) {
        fclose(f);
        printf("File '%s' already exists.\n", fileName);
        printf("Do you want to overwrite it? (y/n): ");
        scanf(" %c", &choice);
        getchar();

        if (choice == 'y' || choice == 'Y') {
            f = fopen(fileName, "w"); // ghi đè
            if (f == NULL) {
                printf("Error opening file for writing.\n");
                return;
            }
            printf("🧹 Old data cleared.\n");
        } else {
            f = fopen(fileName, "a"); // ghi nối thêm
            if (f == NULL) {
                printf("Error opening file for appending.\n");
                return;
            }
            append = true;
            printf("📎 Data will be appended to existing file.\n");
        }
    } else {
        f = fopen(fileName, "w"); // tạo mới
        if (f == NULL) {
            printf("Error creating new file.\n");
            return;
        }
        printf("✅ File created: %s\n", fileName);
    }

void calculateMaxLengths(Student list[], int n, int *maxName, int *maxEmail) {
    *maxName = 8;   
    *maxEmail = 12; 

    for (int i = 0; i < n; i++) {
        int lenName = strlen(list[i].name);
        int lenEmail = strlen(list[i].email);

        if (lenName > *maxName)
            *maxName = lenName;

        if (lenEmail > *maxEmail)
            *maxEmail = lenEmail;
    }
}

// Hàm căn giữa chuỗi 
void centerText(char *dest, const char *src, int width) {
    int len = strlen(src);
    int padding = (width - len) / 2;
    int left = padding > 0 ? padding : 0;
    int right = width - len - left;
    sprintf(dest, "%*s%s%*s", left, "", src, right, "");
}

// Hàm ghi dữ liệu ra file .txt
void saveTextFile(Student list[], int n, int maxName, int maxEmail) {
    char fileName[100];
    printf("Enter file name to save (e.g. students.txt): ");
    fgets(fileName, sizeof(fileName), stdin);
    fileName[strcspn(fileName, "\n")] = '\0'; // Xóa ký tự Enter

    FILE *f = fopen(fileName, "w");
    if (!f) {
        printf("Error opening file!\n");
        return;
    }

    // Header
    int totalWidth = 80 + maxName + maxEmail;
    for (int i = 0; i < totalWidth; i++) fprintf(f, "=");
    fprintf(f, "\n");

    // In dòng tiêu đề
    char temp[200];
    fprintf(f, "| %5s | %10s | %-*s | %8s | %10s | %14s | %-*s | %8s |\n",
            "No.", "ID", maxName, "Name", "Gender", "Class", "Date of Birth", maxEmail, "Email", "Score");

    for (int i = 0; i < totalWidth; i++) fprintf(f, "-");
    fprintf(f, "\n");

    // In từng học sinh
    for (int i = 0; i < n; i++) {
        fprintf(f, "| %5d | %10s | %-*s | %8s | %10s | %14s | %-*s | %8.2f |\n",
                i + 1,
                list[i].id,
                maxName, list[i].name,
                list[i].gender,
                list[i].className,
                list[i].dob,
                maxEmail, list[i].email,
                list[i].score);
    }

    // Footer
    for (int i = 0; i < totalWidth; i++) fprintf(f, "=");
    fprintf(f, "\n");

    fclose(f);
    printf("Data saved successfully to '%s'.\n", fileName);
}

// ------------------ Hàm cắt khoảng trắng ------------------
void trim(char *str) {
    // Cắt khoảng trắng đầu
    while (*str == ' ' || *str == '\t') str++;

    // Cắt khoảng trắng cuối
    char *end = str + strlen(str) - 1;
    while (end > str && (*end == ' ' || *end == '\n' || *end == '\r' || *end == '\t'))
        *end-- = '\0';
}

// ------------------ Hàm đọc file .txt ------------------
void loadTextFile(Student list[], int *n) {
    char fileName[100];
    printf("Enter file name to load: ");
    fgets(fileName, sizeof(fileName), stdin);
    fileName[strcspn(fileName, "\n")] = '\0'; // xóa ký tự Enter

    FILE *f = fopen(fileName, "r");
    if (!f) {
        printf("❌ File not found!\n");
        return;
    }

    *n = 0; // Xóa dữ liệu cũ trong bộ nhớ
    printf("Loading data from '%s'...\n", fileName);

    char line[512];

    // Bỏ qua 3 dòng đầu (header, tiêu đề, gạch ngang)
    fgets(line, sizeof(line), f);
    fgets(line, sizeof(line), f);
    fgets(line, sizeof(line), f);

    while (fgets(line, sizeof(line), f)) {
        // Bỏ qua dòng phân cách hoặc trống
        if (line[0] == '=' || line[0] == '-' || line[0] == '\n')
            continue;

        // Tách cột theo dấu '|'
        char *parts[10];
        int count = 0;

        char *token = strtok(line, "|");
        while (token != NULL && count < 10) {
            trim(token);
            parts[count++] = token;
            token = strtok(NULL, "|");
        }

        // Dữ liệu chuẩn có ít nhất 9 cột (No, ID, Name, Gender, Class, DOB, Email, Score)
        if (count < 9) continue;

        // Gán dữ liệu vào struct Student
        Student st;
        strcpy(st.id, parts[2]);
        strcpy(st.name, parts[3]);
        strcpy(st.gender, parts[4]);
        strcpy(st.className, parts[5]);
        strcpy(st.dob, parts[6]);
        strcpy(st.email, parts[7]);
        st.score = atof(parts[8]); // chuyển chuỗi -> số

        list[*n] = st;
        (*n)++;
    }

    fclose(f);
    printf("✅ File loaded successfully! %d students imported.\n", *n);
}

// ------------------ Hàm lưu CSV ------------------
void saveCSV(Student list[], int n) {
    char fileName[100];
    printf("Enter CSV file name to save (e.g. students.csv): ");
    fgets(fileName, sizeof(fileName), stdin);
    fileName[strcspn(fileName, "\n")] = '\0'; // xoá ký tự Enter

    // Thêm đuôi .csv nếu chưa có
    if (strstr(fileName, ".csv") == NULL) {
        strcat(fileName, ".csv");
    }

    FILE *f;
    int append = 0;
    char choice[10];

    // Kiểm tra xem file đã tồn tại chưa
    f = fopen(fileName, "r");
    if (f) {
        fclose(f);
        printf("⚠️ File '%s' already exists.\n", fileName);
        printf("Do you want to overwrite it? (y/n): ");
        fgets(choice, sizeof(choice), stdin);
        choice[strcspn(choice, "\n")] = '\0';

        if (choice[0] == 'y' || choice[0] == 'Y') {
            f = fopen(fileName, "w"); // ghi đè
            printf("🧹 Old data cleared.\n");
        } else {
            f = fopen(fileName, "a"); // nối thêm
            append = 1;
            printf("📎 Data will be appended to existing file.\n");
        }
    } else {
        f = fopen(fileName, "w");
        if (!f) {
            printf("❌ Error: cannot create file!\n");
            return;
        }
        printf("✅ File created: %s\n", fileName);
    }

    // Ghi header nếu không phải append
    if (!append) {
        fprintf(f, "No.,ID,Name,Gender,Class,Date of Birth,Email,Score\n");
    }

    // Ghi từng học sinh
    for (int i = 0; i < n; i++) {
        // Xử lý tên: loại bỏ dấu phẩy (,) để tránh lỗi định dạng CSV
        char safeName[100];
        strcpy(safeName, list[i].name);
        for (int j = 0; safeName[j]; j++) {
            if (safeName[j] == ',') safeName[j] = ' ';
        }

        fprintf(f, "%d,%s,%s,%s,%s,%s,%s,%.2f\n",
                i + 1,
                list[i].id,
                safeName,
                list[i].gender,
                list[i].className,
                list[i].dob,
                list[i].email,
                list[i].score);
    }

    fclose(f);
    printf("💾 Data exported successfully to CSV file: %s\n", fileName);

    // Mở file sau khi lưu (tùy hệ điều hành)
#ifdef _WIN32
    printf("📂 Opening file...\n");
    char command[200];
    sprintf(command, "start %s", fileName);
    system(command);
#elif __APPLE__
    char command[200];
    sprintf(command, "open %s", fileName);
    system(command);
#elif __linux__
    char command[200];
    sprintf(command, "xdg-open %s", fileName);
    system(command);
#else
    printf("⚠️ Auto-open not supported on this system.\n");
#endif
}

// ------------------ Hàm đọc CSV ------------------
void loadCSVFile(Student list[], int *n) {
    char fileName[100];
    printf("Enter CSV file name to load (e.g. students.csv): ");
    fgets(fileName, sizeof(fileName), stdin);
    fileName[strcspn(fileName, "\n")] = '\0'; // xoá ký tự Enter

    // Tự động thêm đuôi .csv nếu chưa có
    if (strstr(fileName, ".csv") == NULL) {
        strcat(fileName, ".csv");
    }

    FILE *f = fopen(fileName, "r");
    if (!f) {
        printf("❌ File not found!\n");
        return;
    }

    char line[256];
    *n = 0; // Xóa dữ liệu cũ trong danh sách

    // Bỏ qua dòng tiêu đề (header)
    if (fgets(line, sizeof(line), f) == NULL) {
        printf("⚠️ File is empty!\n");
        fclose(f);
        return;
    }

    // Đọc từng dòng dữ liệu
    while (fgets(line, sizeof(line), f)) {
        if (strlen(line) < 5) continue; // bỏ dòng trống hoặc sai định dạng

        // Xóa ký tự xuống dòng cuối chuỗi
        line[strcspn(line, "\n")] = '\0';

        // Tách cột theo dấu phẩy
        char *parts[8];
        int i = 0;
        char *token = strtok(line, ",");
        while (token != NULL && i < 8) {
            parts[i++] = token;
            token = strtok(NULL, ",");
        }

        // Cần đủ 8 cột dữ liệu (No., ID, Name, Gender, Class, DOB, Email, Score)
        if (i < 8) continue;

        // Gán vào struct Student
        strcpy(list[*n].id, parts[1]);
        strcpy(list[*n].name, parts[2]);
        strcpy(list[*n].gender, parts[3]);
        strcpy(list[*n].className, parts[4]);
        strcpy(list[*n].dob, parts[5]);
        strcpy(list[*n].email, parts[6]);
        list[*n].score = atof(parts[7]);

        (*n)++; // tăng số lượng học sinh
    }

    fclose(f);
    printf("✅ Loaded successfully! Total students: %d\n", *n);
}