#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include "headers/students.h"

// ---------------------------------------------------------------------------------------------------------------------
int main() {
  settings_screen();
  Student students[MAX_STUDENTS];
  initialize_database(students);
  clear_screen();

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
            if (get_user_choice() != 1) break;
          }
        }
        continue_program();
        break;
      case 2:
        print_database(students);
        break;
      case 3:
        add_student(students);
        continue_program();
        break;
      case 4:
        sort_students_by_gpa_and_print(students);
        break;
      case 5:
        delete_student(students);
        continue_program();
        break;
      case 6:
        save_database(students);
        continue_program();
        break;
      case 7:
        load_database(students);
        continue_program();
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
          char *newline = strrchr(column, '\n');
          if (newline) {
            *newline = '\0'; // Удаление символа новой строки
          }

          if (strcmp(column, "faculty") == 0 || strcmp(column, "name") == 0 || strcmp(column, "surname") == 0) {
            printf("Введите начальное значение для поиска: ");
            fgets(str_from, 50, stdin);
            newline = strrchr(str_from, '\n');
            if (newline) {
              *newline = '\0'; // Удаление символа новой строки
            }
            printf("Введите конечное значение для поиска: ");
            fgets(str_to, 50, stdin);
            newline = strrchr(str_to, '\n');
            if (newline) {
              *newline = '\0'; // Удаление символа новой строки
            }
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
        continue_program();
        break;
      }
      case 99:
        printf("Выход из программы.\n");
        exit(0);
      default:
        printf("Неверный выбор. Попробуйте еще раз.\n");
    }
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
  printf("4. Сортировать студентов по среднему баллу\n");
  printf("5. Удалить студента по ID\n");
  printf("6. Сохранение базы данных студентов\n");
  printf("7. Загрузка базы данных студентов\n");
  printf("8. Поиск студентов по столбцу с ограничением от и до\n");
  printf("99. Выход\n");
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
    students[i].id = 0;
    students[i].original_index = 0;
    students[i].name[0] = '\0';
    students[i].surname[0] = '\0';
    students[i].age = 0;
    students[i].faculty[0] = '\0';
    students[i].gpa = 0.0;
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

// -----Добавление студента в базу данных-----
int input_id(Student students[]) {
  int id;
  char id_str[50];
  do {
    printf("Введите ID студента: ");
    user_input();
    scanf_s("%d", &id);
    clear_input_buffer();
    to_string(id, id_str);
    if (id <= 0 || strlen(id_str) > 6 || id_exists(students, id)) {
      printf(
        "ID студента должен быть больше 0, содержать не более 6 символов и быть уникальным. Попробуйте еще раз.\n\n");
    } else {
      break;
    }
  } while (1);
  return id;
}

void input_name(char *name) {
  do {
    printf("\nВведите имя студента: ");
    user_input();
    fgets(name, 50, stdin);
    char *newline = strrchr(name, '\n');
    if (newline) {
      *newline = '\0'; // Удаление символа новой строки
    }
    if (strlen(name) == 0 || strlen(name) > 20) {
      printf("Имя студента не может быть пустым или содержать больше 20 символов. Попробуйте еще раз.\n\n");
    } else {
      break;
    }
  } while (1);
}

void input_surname(char *surname) {
  do {
    printf("\nВведите фамилию студента: ");
    user_input();
    fgets(surname, 50, stdin);
    char *newline = strrchr(surname, '\n');
    if (newline) {
      *newline = '\0'; // Удаление символа новой строки
    }
    if (strlen(surname) == 0 || strlen(surname) > 20) {
      printf("Фамилия студента не может быть пустой или содержать больше 20 символов. Попробуйте еще раз.\n\n");
    } else {
      break;
    }
  } while (1);
}

void input_age(int *age) {
  do {
    printf("\nВведите возраст студента: ");
    user_input();
    scanf_s("%d", age);
    clear_input_buffer();
    if (*age <= 16 || *age > 99) {
      printf("Возраст студента должен быть больше 16 и меньше 99. Попробуйте еще раз.\n\n");
    } else {
      break;
    }
  } while (1);
}

void input_faculty(char *faculty) {
  do {
    printf("\nВведите факультет студента: ");
    user_input();
    fgets(faculty, 100, stdin);
    char *newline = strrchr(faculty, '\n');
    if (newline) {
      *newline = '\0'; // Удаление символа новой строки
    }
    if (strlen(faculty) == 0 || strlen(faculty) > 20) {
      printf("Факультет студента не может быть пустым или содержать больше 20 символов. Попробуйте еще раз.\n\n");
    } else {
      break;
    }
  } while (1);
}

void input_gpa(double *gpa) {
  do {
    printf("\nВведите средний балл студента: ");
    user_input();
    scanf_s("%lf", gpa);
    clear_input_buffer();
    if (*gpa <= 0 || *gpa > 54) {
      printf("Средний балл студента должен быть больше 0 и меньше или равен 54. Попробуйте еще раз.\n\n");
    } else {
      break;
    }
  } while (1);
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

  students[index] = new_student;
  printf("Студент успешно добавлен.\n\n");
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
  continue_program();
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

// Сортировка студентов по среднему баллу
void sort_students_by_gpa_and_print(Student students[]) {
  // Создаем временный массив студентов
  Student temp_students[MAX_STUDENTS];
  // Копируем исходные данные во временный массив
  for (int i = 0; i < MAX_STUDENTS; i++) {
    temp_students[i] = students[i];
  }

  // Используем пузырьковую сортировку для сортировки временного массива
  for (int i = 0; i < MAX_STUDENTS - 1; i++) {
    for (int j = 0; j < MAX_STUDENTS - i - 1; j++) {
      if (temp_students[j].gpa > temp_students[j + 1].gpa) {
        Student temp = temp_students[j];
        temp_students[j] = temp_students[j + 1];
        temp_students[j + 1] = temp;
      }
    }
  }

  // Выводим отсортированный временный массив
  print_database(temp_students);
}

// Настройка консоли
void settings_screen() {
  SetConsoleCP(CP_UTF8);
  SetConsoleOutputCP(CP_UTF8);
  SetConsoleTitle("OSDB");
}

// Преобразование числа в строку
void to_string(int num, char *str) {
  sprintf(str, "%d", num);
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

// Поиск студентов по столбцу
void search_students(Student students[], SearchFilter filters[], int filters_count) {
  Student temp_students[MAX_STUDENTS];
  int temp_index = 0;

  for (int i = 0; i < MAX_STUDENTS; i++) {
    if (strlen(students[i].name) != 0) {
      int matches_all_filters = 1;
      for (int j = 0; j < filters_count; j++) {
        SearchFilter filter = filters[j];
        if (!(
          (strcmp(filter.column, "id") == 0 && students[i].id >= filter.num_from && students[i].id <= filter.num_to) ||
          (strcmp(filter.column, "age") == 0 && students[i].age >= filter.num_from &&
           students[i].age <= filter.num_to) ||
          (strcmp(filter.column, "gpa") == 0 && students[i].gpa >= filter.num_from &&
           students[i].gpa <= filter.num_to) ||
          (strcmp(filter.column, "faculty") == 0 &&
           (strncasecmp(students[i].faculty, filter.str_from, strlen(filter.str_from)) >= 0 &&
            strncasecmp(students[i].faculty, filter.str_to, strlen(filter.str_to)) <= 0)) ||
          (strcmp(filter.column, "name") == 0 &&
           (strncasecmp(students[i].name, filter.str_from, strlen(filter.str_from)) >= 0 &&
            strncasecmp(students[i].name, filter.str_to, strlen(filter.str_to)) <= 0)) ||
          (strcmp(filter.column, "surname") == 0 &&
           (strncasecmp(students[i].surname, filter.str_from, strlen(filter.str_from)) >= 0 &&
            strncasecmp(students[i].surname, filter.str_to, strlen(filter.str_to)) <= 0)))) {
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
  print_database_header();
  for (int i = 0; i < temp_index; i++) {
    print_student(temp_students[i]);
  }
  print_database_footer();
}