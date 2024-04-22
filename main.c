#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <ctype.h>
#include "headers/students.h"

// ---------------------------------------------------------------------------------------------------------------------
int main() {
  settings_screen();
  Student students[MAX_STUDENTS];
  initialize_database(students);
  clear_screen();

  int is_pg = 0;

  while (1) {
    print_menu();
    int choice = get_user_choice();
    clear_screen();
    switch (choice) {
      case 1:
        printf("Вы уверены, что хотите очистить базу данных и ввести новых студентов? (1 - да, 0 - нет): ");
        if (get_user_choice() != 1) {
          continue_program();
          continue;
        } else {
          initialize_database(students);
          printf("База данных очищена и готова к вводу.\n");
          while (1) {
            add_student(students);
            printf("Добавить еще одного студента? (1 - да, 0 - нет): ");
            if (get_user_choice() != 1) {
              is_pg = 1;
              break;
            }
          }
        }
        break;
      case 7:
        load_database(students);
        is_pg = 1;
        break;
      case 9:
        printf("Выход из программы.\n");
        exit(0);
      default:
        if (!is_pg) {
          printf("База данных пуста. Пожалуйста, введите студентов перед использованием других функций.\n");
          continue;
        }
        switch (choice) {
          case 2:
            print_database(students);
            break;
          case 3:
            add_student(students);
            break;
          case 4: {
            char column[50];
            int order;
            printf("Введите столбец для сортировки (id, age, gpa, faculty, name, surname): ");
            fgets(column, 50, stdin);
            column[strcspn(column, "\n")] = 0; // Удаление символа новой строки
            printf("Введите порядок сортировки (1 - по возрастанию, 0 - по убыванию): ");
            scanf_s("%d", &order);
            clear_input_buffer();
            sort_students(students, column, order);
            break;
          }
          case 5:
            delete_student(students);
            break;
          case 6:
            save_database(students);
            break;
          case 8: {
            SearchFilter filters[MAX_STUDENTS];
            int filters_count = 0;

            while (1) {
              char column[50];
              char str_from[50] = "", str_to[50] = "";
              double num_from = 0, num_to = 0;

              printf("Введите столбец для поиска (id, age, gpa, faculty, name, surname): ");
              fgets(column, 50, stdin);
              column[strcspn(column, "\n")] = 0; // Удаление символа новой строки

              if (strcmp(column, "faculty") == 0 || strcmp(column, "name") == 0 || strcmp(column, "surname") == 0) {
                printf("Введите начальное значение для поиска: ");
                fgets(str_from, 50, stdin);
                str_from[strcspn(str_from, "\n")] = 0; // Удаление символа новой строки

                printf("Введите конечное значение для поиска: ");
                fgets(str_to, 50, stdin);
                str_to[strcspn(str_to, "\n")] = 0; // Удаление символа новой строки
              } else {
                printf("Введите нижнюю границу диапазона: ");
                scanf_s("%lf", &num_from);
                printf("Введите верхнюю границу диапазона: ");
                scanf_s("%lf", &num_to);
                clear_input_buffer();
              }

              SearchFilter filter;
              strcpy(filter.column, column);
              strcpy(filter.str_from, str_from);
              strcpy(filter.str_to, str_to);
              filter.num_from = num_from;
              filter.num_to = num_to;

              filters[filters_count++] = filter;

              printf("Добавить еще один фильтр? (1 - да, 0 - нет): ");
              if (get_user_choice() != 1) {
                printf("\nВсе фильтры добавлены.\n");
                break;
              } else {
                printf("\n");
              }
            }

            search_students(students, filters, filters_count);
            break;
          }
          default:
            printf("Неверный выбор. Попробуйте еще раз.\n");
        }
    }
    continue_program();
  }
}
// ---------------------------------------------------------------------------------------------------------------------

// Ввод пользователя
void user_input() {
  printf("\n\n$user ~:");
}

// Вывод меню
void print_menu() {
  printf("\nOwn Shit Data Base\n\n");
  printf("1. Ввод студентов в базу данных (с очисткой)\n");
  printf("2. Вывод базы данных\n");
  printf("3. Добавить студента\n");
  printf("4. Сортировка по любому столбцу\n");
  printf("5. Удалить студента по ID\n");
  printf("6. Сохранение базы данных студентов\n");
  printf("7. Загрузка базы данных студентов\n");
  printf("8. Поиск студентов по столбцу с ограничением от и до\n");
  printf("9. Выход\n");
  user_input();
}

// Получение выбора пользователя
int get_user_choice() {
  int choice;
  scanf_s("%d", &choice);
  clear_input_buffer();
  return choice;
}

// Очистка буфера ввода
void clear_input_buffer() {
  int c;
  while ((c = getchar()) != '\n' && c != EOF) {}
}

// Очистка экрана
void clear_screen() {
  system("cls");
  printf("\033[H\033[J");
}

// Продолжение программы
void continue_program() {
  printf("Нажмите Enter для продолжения...");
  clear_input_buffer();
  clear_screen();
}

// Инициализация базы данных
void initialize_database(Student students[]) {
  for (int i = 0; i < MAX_STUDENTS; i++) {
    memset(&students[i], 0, sizeof(Student));
  }
}

// Поиск свободного места в базе данных
int find_free_slot(Student students[]) {
  for (int i = 0; i < MAX_STUDENTS; i++) {
    if (strlen(students[i].name) == 0) {
      return i;
    }
  }
  return -1;
}

// Проверка на то, что строка состоит только из цифр
int is_all_digits(const char *str) {
  for (int i = 0; str[i] != '\0'; i++) {
    if (!isdigit(str[i])) {
      return 0;
    }
  }
  return 1;
}

// -----Добавление студента в базу данных-----
int input_id(Student students[]) {
  clear_screen();
  char input[50];
  char *endptr;
  do {
    printf("Введите ID( студак ) студента: ");
    fgets(input, 50, stdin);
    input[strcspn(input, "\n")] = 0; // Удаление символа новой строки

    if (is_all_digits(input)) {
      long id = strtol(input, &endptr, 10);
      if (*endptr != '\0' || id <= 0 || strlen(input) > 6 || id_exists(students, id)) {
        printf(
          "ID студента должен быть больше 0, содержать не более 6 символов и быть уникальным. Попробуйте еще раз.\n\n");
      } else {
        clear_screen();
        return (int) id;
      }
    } else {
      printf("ID студента должен быть числом. Попробуйте еще раз.\n\n");
    }
  } while (1);
}

void input_name(char *name) {
  do {
    printf("Введите имя студента: ");
    fgets(name, 50, stdin);
    name[strcspn(name, "\n")] = 0; // Удаление символа новой строки
    if (strlen(name) == 0 || strlen(name) > 20) {
      printf("Имя студента не может быть пустым или содержать больше 20 символов. Попробуйте еще раз.\n\n");
    } else {
      break;
    }
  } while (1);
  clear_screen();
}

void input_surname(char *surname) {
  do {
    printf("Введите фамилию студента: ");
    fgets(surname, 50, stdin);
    surname[strcspn(surname, "\n")] = 0; // Удаление символа новой строки
    if (strlen(surname) == 0 || strlen(surname) > 20) {
      printf("Фамилия студента не может быть пустой или содержать больше 20 символов. Попробуйте еще раз.\n\n");
    } else {
      break;
    }
  } while (1);
  clear_screen();
}

void input_age(int *age) {
  char input[50];
  char *endptr;
  do {
    printf("Введите возраст студента: ");
    fgets(input, 50, stdin);
    input[strcspn(input, "\n")] = 0; // Удаление символа новой строки

    if (is_all_digits(input)) {
      long temp_age = strtol(input, &endptr, 10);
      if (*endptr != '\0' || temp_age <= 16 || temp_age > 99) {
        printf("Возраст студента должен быть больше 16 и меньше 99. Попробуйте еще раз.\n\n");
      } else {
        clear_screen();
        *age = (int) temp_age;
        return;
      }
    } else {
      printf("Возраст студента должен быть числом. Попробуйте еще раз.\n\n");
    }
  } while (1);
}

void input_faculty(char *faculty) {
  do {
    printf("Введите факультет студента: ");
    fgets(faculty, 100, stdin);
    faculty[strcspn(faculty, "\n")] = 0; // Удаление символа новой строки
    if (strlen(faculty) == 0 || strlen(faculty) > 20) {
      printf("Факультет студента не может быть пустым или содержать больше 20 символов. Попробуйте еще раз.\n\n");
    } else {
      break;
    }
  } while (1);
  clear_screen();
}

void input_gpa(double *gpa) {
  char input[50];
  char *endptr;
  do {
    printf("Введите средний балл студента: ");
    fgets(input, 50, stdin);
    input[strcspn(input, "\n")] = 0; // Удаление символа новой строки

    double temp_gpa = strtod(input, &endptr);
    if (*endptr != '\0' || temp_gpa <= 0.0 || temp_gpa > 54.0) {
      printf("Средний балл студента должен быть больше 0.0 и меньше или равен 54. Попробуйте еще раз.\n\n");
    } else {
      clear_screen();
      *gpa = temp_gpa;
      return;
    }
  } while (1);
}

void confirm_and_add_student(Student students[], Student new_student) {
  printf("\nПроверьте введенные данные:\n");
  printf("ID студента: %d\n", new_student.id);
  printf("Имя студента: %s\n", new_student.name);
  printf("Фамилия студента: %s\n", new_student.surname);
  printf("Возраст студента: %d\n", new_student.age);
  printf("Факультет студента: %s\n", new_student.faculty);
  printf("Средний балл студента: %.2f\n\n", new_student.gpa);

  printf("Все верно? (1 - да, 0 - нет): ");
  if (get_user_choice() == 1) {
    students[new_student.original_index] = new_student;
    printf("Студент успешно добавлен.\n\n");
  } else {
    printf("Студент не добавлен.\n\n");
  }
}

void add_student(Student students[]) {
  int index = find_free_slot(students);
  if (index == -1) {
    printf("База данных заполнена. Невозможно добавить студента.\n");
    return;
  }

  Student new_student;
  new_student.id = input_id(students);
  new_student.original_index = index;
  input_name(new_student.name);
  input_surname(new_student.surname);
  input_age(&new_student.age);
  input_faculty(new_student.faculty);
  input_gpa(&new_student.gpa);

  confirm_and_add_student(students, new_student);
}
// -----Добавление студента в базу данных-----

// -----Вывод базы данных-----
void print_student(Student student) {
  printf("%4d | %6d | %-20s | %-20s | %10d | %-20s | %12.2f |\n", student.original_index + 1, student.id, student.name,
         student.surname,
         student.age, student.faculty, student.gpa);
}

void print_database_header() {
  printf(
    "------------------------------------------------------------------------------------------------------------------------\n");
  printf(
    "   № |     ID | Имя студента         | Фамилия студента     | Возраст    | Факультет            | Средний балл |\n");
  printf(
    "------------------------------------------------------------------------------------------------------------------------\n");
}

void print_database_footer() {
  printf(
    "------------------------------------------------------------------------------------------------------------------------\n");
}

void print_database(Student students[]) {
  print_database_header();
  for (int i = 0; i < MAX_STUDENTS; i++) {
    if (strlen(students[i].name) != 0) {
      print_student(students[i]);
    }
  }
  print_database_footer();
}
// -----Вывод базы данных-----

// Удаление студента по ID
void delete_student(Student students[]) {
  int id_to_delete;
  printf("Введите ID студента для удаления: ");
  scanf_s("%d", &id_to_delete);
  clear_input_buffer();

  for (int i = 0; i < MAX_STUDENTS; i++) {
    if (students[i].id == id_to_delete) {
      students[i].id = 0;
      students[i].original_index = 0;
      students[i].name[0] = '\0';
      students[i].surname[0] = '\0';
      students[i].age = 0;
      students[i].faculty[0] = '\0';
      students[i].gpa = 0.0;
      printf("Студент с ID %d удален.\n", id_to_delete);
      return;
    }
  }
  printf("Студент с ID %d не найден.\n", id_to_delete);
}

// Настройка консоли
void settings_screen() {
  SetConsoleCP(CP_UTF8);
  SetConsoleOutputCP(CP_UTF8);
  SetConsoleTitle("OSDB");
}

// Проверка наличия ID в базе данных
int id_exists(Student students[], int id) {
  for (int i = 0; i < MAX_STUDENTS; i++) {
    if (students[i].id == id) {
      return 1;
    }
  }
  return 0;
}

// Сохранение и загрузка базы данных
void save_database(Student students[]) {
  FILE *file = fopen("../db.dat", "wb");
  if (file == NULL) {
    printf("Не удалось открыть файл для записи.\n");
    return;
  }

  for (int i = 0; i < MAX_STUDENTS; i++) {
    if (students[i].id != 0) {
      fwrite(&students[i], sizeof(Student), 1, file);
    }
  }

  fclose(file);
  printf("База данных успешно сохранена в файл db.dat.\n");
}

void load_database(Student students[]) {
  FILE *file = fopen("../db.dat", "rb");
  if (file == NULL) {
    printf("Не удалось открыть файл для чтения.\n");
    return;
  }

  printf(
    "Осторожно! Загрузка базы данных из файла db.dat приведет к полной очистке текущей базы данных.\nВы уверены? (1 - да, 0 - нет): ");
  if (get_user_choice() != 1) {
    fclose(file);
    return;
  }

  int i = 0;
  while (fread(&students[i], sizeof(Student), 1, file) == 1 && i < MAX_STUDENTS) {
    i++;
  }

  fclose(file);
  printf("База данных успешно загружена из файла db.dat.\n");
}

// Проверка на соответствие фильтру
int matches_filter(Student student, SearchFilter filter) {
  if (strcmp(filter.column, "id") == 0) {
    return student.id >= filter.num_from && student.id <= filter.num_to;
  } else if (strcmp(filter.column, "age") == 0) {
    return student.age >= filter.num_from && student.age <= filter.num_to;
  } else if (strcmp(filter.column, "gpa") == 0) {
    return student.gpa >= filter.num_from && student.gpa <= filter.num_to;
  } else if (strcmp(filter.column, "faculty") == 0) {
    return strncasecmp(student.faculty, filter.str_from, strlen(filter.str_from)) >= 0 &&
           strncasecmp(student.faculty, filter.str_to, strlen(filter.str_to)) <= 0;
  } else if (strcmp(filter.column, "name") == 0) {
    return strncasecmp(student.name, filter.str_from, strlen(filter.str_from)) >= 0 &&
           strncasecmp(student.name, filter.str_to, strlen(filter.str_to)) <= 0;
  } else if (strcmp(filter.column, "surname") == 0) {
    return strncasecmp(student.surname, filter.str_from, strlen(filter.str_from)) >= 0 &&
           strncasecmp(student.surname, filter.str_to, strlen(filter.str_to)) <= 0;
  }
  return 0;
}

// Поиск студентов по столбцу
void search_students(Student students[], SearchFilter filters[], int filters_count) {
  Student temp_students[MAX_STUDENTS];
  memset(temp_students, 0, sizeof(temp_students));
  int temp_index = 0;

  for (int i = 0; i < MAX_STUDENTS; i++) {
    if (strlen(students[i].name) != 0) {
      int matches_all_filters = 1;
      for (int j = 0; j < filters_count; j++) {
        if (!matches_filter(students[i], filters[j])) {
          matches_all_filters = 0;
          break;
        }
      }
      if (matches_all_filters) {
        temp_students[temp_index++] = students[i];
      }
    }
  }

  printf("Поиск студентов по заданным фильтрам:\n");
  print_database(temp_students);
}

// Проверка на необходимость смены местами
int should_swap(Student a, Student b, char column[50], int order) {
  if (strcmp(column, "id") == 0) {
    return order == 0 ? a.id > b.id : a.id < b.id;
  } else if (strcmp(column, "age") == 0) {
    return order == 0 ? a.age > b.age : a.age < b.age;
  } else if (strcmp(column, "gpa") == 0) {
    return order == 0 ? a.gpa > b.gpa : a.gpa < b.gpa;
  } else if (strcmp(column, "faculty") == 0) {
    return order == 0 ? strcmp(a.faculty, b.faculty) > 0 : strcmp(a.faculty, b.faculty) < 0;
  } else if (strcmp(column, "name") == 0) {
    return order == 0 ? strcmp(a.name, b.name) > 0 : strcmp(a.name, b.name) < 0;
  } else if (strcmp(column, "surname") == 0) {
    return order == 0 ? strcmp(a.surname, b.surname) > 0 : strcmp(a.surname, b.surname) < 0;
  }
  return 0;
}

// Сортировка студентов по столбцу
void sort_students(Student students[], char column[50], int order) {
  Student temp_students[MAX_STUDENTS];
  for (int i = 0; i < MAX_STUDENTS; i++) {
    temp_students[i] = students[i];
  }

  for (int i = 0; i < MAX_STUDENTS - 1; i++) {
    for (int j = 0; j < MAX_STUDENTS - i - 1; j++) {
      if (should_swap(temp_students[j], temp_students[j + 1], column, order)) {
        Student temp = temp_students[j];
        temp_students[j] = temp_students[j + 1];
        temp_students[j + 1] = temp;
      }
    }
  }

  print_database(temp_students);
}