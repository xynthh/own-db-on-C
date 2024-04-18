#ifndef MAIN_C_STUDENTS_H
#define MAIN_C_STUDENTS_H

#define MAX_STUDENTS 128

typedef struct {
    int id; // ID студента
    int original_index; // Индекс студента в массиве
    char name[50]; // Имя студента
    char surname[50]; // Фамилия студента
    int age; // Возраст студента
    char faculty[100]; // Факультет студента
    double gpa; // Средний балл студента
} Student;

typedef struct {
    char column[50];
    char str_from[50];
    char str_to[50];
    double num_from;
    double num_to;
} SearchFilter;

void user_input();
void print_menu();
int get_user_choice();
void clear_input_buffer();
void clear_screen();
void continue_program();
void initialize_database(Student students[]);
int find_free_slot(Student students[]);
int input_id(Student students[]);
void input_name(char *name);
void input_surname(char *surname);
void input_age(int *age);
void input_faculty(char *faculty);
void input_gpa(double *gpa);
void add_student(Student students[]);
void print_student(Student student);
void print_database_header();
void print_database_footer();
void print_database(Student students[]);
void delete_student(Student students[]);
void sort_students_by_gpa_and_print(Student students[]);
void settings_screen();
void to_string(int, char *str);
int id_exists(Student students[], int);
void save_database(Student students[]);
void load_database(Student students[]);
void search_students(Student students[], SearchFilter filters[], int filters_count);

#endif //MAIN_C_STUDENTS_H