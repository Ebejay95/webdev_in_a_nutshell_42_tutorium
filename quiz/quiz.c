#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_QUESTION_LENGTH 256
#define MAX_OPTION_LENGTH 128
#define NUM_OPTIONS 4
#define MAX_QUESTIONS 50

// ANSI Color codes
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"

// Structure for quiz statistics
typedef struct {
    int correct_answers;
    int total_questions;
    float score_percentage;
} QuizStats;

// Structure for quiz questions
typedef struct {
    char question[MAX_QUESTION_LENGTH];
    char options[NUM_OPTIONS][MAX_OPTION_LENGTH];
    int correct_answer;
    char explanation[MAX_QUESTION_LENGTH];
} Question;

// Function to load questions from file
Question* load_questions(const char *filename, int *num_questions) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, RED "Error: Cannot open file %s\n" RESET, filename);
        exit(1);
    }

    Question *questions = malloc(sizeof(Question) * MAX_QUESTIONS);
    *num_questions = 0;
    char line[MAX_QUESTION_LENGTH];

    while (*num_questions < MAX_QUESTIONS) {
        // Read question
        if (fgets(line, sizeof(line), file) == NULL) break;
        line[strcspn(line, "\n")] = 0;
        if (strlen(line) == 0) continue;  // Skip empty lines
        strcpy(questions[*num_questions].question, line);

        // Read options
        for (int i = 0; i < NUM_OPTIONS; i++) {
            if (fgets(line, sizeof(line), file) == NULL) {
                fprintf(stderr, RED "Error: Incomplete question data\n" RESET);
                exit(1);
            }
            line[strcspn(line, "\n")] = 0;
            snprintf(questions[*num_questions].options[i], MAX_OPTION_LENGTH, 
                    "%d. %s", i + 1, line);
        }

        // Read correct answer
        if (fgets(line, sizeof(line), file) == NULL) {
            fprintf(stderr, RED "Error: Missing correct answer\n" RESET);
            exit(1);
        }
        questions[*num_questions].correct_answer = atoi(line);

        // Read explanation
        if (fgets(line, sizeof(line), file) == NULL) {
            fprintf(stderr, RED "Error: Missing explanation\n" RESET);
            exit(1);
        }
        line[strcspn(line, "\n")] = 0;
        strcpy(questions[*num_questions].explanation, line);

        (*num_questions)++;
    }

    fclose(file);
    return questions;
}

// Function to shuffle questions
void shuffle_questions(Question *questions, int num_questions) {
    for (int i = num_questions - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Question temp = questions[i];
        questions[i] = questions[j];
        questions[j] = temp;
    }
}

// Function to clear input buffer
void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Function to validate user input
int get_valid_answer() {
    int answer;
    char line[10];
    
    while (1) {
        if (fgets(line, sizeof(line), stdin)) {
            if (sscanf(line, "%d", &answer) == 1) {
                if (answer >= 1 && answer <= 4) {
                    return answer;
                }
            }
        }
        printf(YELLOW "Please enter a valid number between 1 and 4: " RESET);
    }
}

// Function to calculate quiz statistics
QuizStats calculate_stats(int correct_answers, int total_questions) {
    QuizStats stats;
    stats.correct_answers = correct_answers;
    stats.total_questions = total_questions;
    stats.score_percentage = ((float)correct_answers / total_questions) * 100;
    return stats;
}

// Function to display quiz statistics
void display_stats(QuizStats stats) {
    printf("\n" BOLD MAGENTA "=== Quiz Results ===" RESET "\n");
    printf(CYAN "Correct Answers: %d/%d\n", stats.correct_answers, stats.total_questions);
    printf("Score: %.1f%%\n" RESET, stats.score_percentage);
    
    if (stats.score_percentage >= 90) {
        printf(GREEN "Excellent! You're a URL expert!\n" RESET);
    } else if (stats.score_percentage >= 70) {
        printf(CYAN "Good job! You have a solid understanding of URLs.\n" RESET);
    } else if (stats.score_percentage >= 50) {
        printf(YELLOW "Not bad! But there's room for improvement.\n" RESET);
    } else {
        printf(RED "Keep studying! URLs are important to understand.\n" RESET);
    }
}

void run_quiz(Question *questions, int num_questions) {
    int correct_answers = 0;
    
    printf("\n" BOLD CYAN "Instructions:" RESET "\n");
    printf(YELLOW "- Choose the correct answer (1-4) for each question\n");
    printf("- You'll get immediate feedback after each answer\n");
    printf("- Your final score will be shown at the end\n\n" RESET);
    
    // Shuffle questions
    shuffle_questions(questions, num_questions);
    
    for (int i = 0; i < num_questions; i++) {
        printf("\n" BOLD BLUE "Question %d of %d:" RESET "\n" BOLD "%s\n\n" RESET, 
               i + 1, num_questions, questions[i].question);
        
        for (int j = 0; j < NUM_OPTIONS; j++) {
            printf(CYAN "%s\n" RESET, questions[i].options[j]);
        }
        
        printf("\n" YELLOW "Your answer (1-4): " RESET);
        int answer = get_valid_answer();
        
        if (answer == questions[i].correct_answer) {
            printf("\n" GREEN "✓ Correct! " RESET);
            correct_answers++;
        } else {
            printf("\n" RED "✗ Wrong! The correct answer was %d. " RESET, questions[i].correct_answer);
        }
        printf(BOLD "%s\n" RESET, questions[i].explanation);
        
        if (i < num_questions - 1) {
            printf("\n" YELLOW "Press Enter to continue..." RESET);
            clear_input_buffer();
            getchar();
        }
    }
    
    // Display final statistics
    QuizStats stats = calculate_stats(correct_answers, num_questions);
    display_stats(stats);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, RED "Usage: %s <questions_file.txt>\n" RESET, argv[0]);
        return 1;
    }

    int num_questions;
    Question *questions = load_questions(argv[1], &num_questions);
    
    char play_again;
    srand((unsigned int)time(NULL));
    
    do {
        printf("\n" BOLD MAGENTA "=== URL Components Quiz ===" RESET "\n");
        run_quiz(questions, num_questions);
        
        printf("\n" YELLOW "Would you like to take the quiz again? (y/n): " RESET);
        scanf(" %c", &play_again);
        clear_input_buffer();
        
    } while (play_again == 'y' || play_again == 'Y');
    
    free(questions);
    printf("\n" GREEN "Thanks for playing! Goodbye!\n" RESET);
    return 0;
}