#ifndef MAIN_C_STUDENTS_H
#define MAIN_C_STUDENTS_H

#define MAX_STUDENTS 128

typedef struct {
    int id;
    int original_index;
    char name[50];
    char surname[50];
    int age;
    char faculty[100];
    double gpa;
} Student;

typedef struct {
    char column[50];
    char str_from[50];
    char str_to[50];
    double num_from;
    double num_to;
} SearchFilter;

// Функции ввода и вывода
void user_input();
void print_menu();
void print_student(Student student);
void print_database_header();
void print_database_footer();
void print_database(Student students[]);

// Функции управления базой данных
void initialize_database(Student students[]);
int find_free_slot(Student students[]);
void add_student(Student students[]);
void delete_student(Student students[]);
void save_database(Student students[]);
void load_database(Student students[]);
void confirm_and_add_student(Student students[], Student new_student);

// Функции ввода данных
int input_id(Student students[]);
void input_name(char *name);
void input_surname(char *surname);
void input_age(int *age);
void input_faculty(char *faculty);
void input_gpa(double *gpa);

// Функции поиска и сортировки
int matches_filter(Student student, SearchFilter filter);
void search_students(Student students[], SearchFilter filters[], int filters_count);
int should_swap(Student a, Student b, char column[50], int order);
void sort_students(Student students[], char column[50], int order);

// Вспомогательные функции
int get_user_choice();
void clear_input_buffer();
void clear_screen();
void continue_program();
void settings_screen();
int is_all_digits(char *str);
int id_exists(Student students[], int id);

#endif //MAIN_C_STUDENTS_H