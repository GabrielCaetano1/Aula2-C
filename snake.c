/*
 * Jogo da Cobrinha (Snake Game) em C
 * Usa a biblioteca ncurses para renderizar no terminal.
 * Funciona perfeitamente em GitHub Codespaces.
 *
 * Controles: W A S D ou setas do teclado
 * Q para sair
 */

#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define MAX_SNAKE_LEN 1000
#define INITIAL_DELAY 120000 // microssegundos entre frames (velocidade inicial)
#define MIN_DELAY 50000      // velocidade máxima

typedef struct {
    int x, y;
} Point;

typedef enum { UP, DOWN, LEFT, RIGHT } Direction;

Point snake[MAX_SNAKE_LEN];
int snake_len;
Direction dir;
Point food;
int score;
int game_over;
int width, height;
int delay_us;

void spawn_food() {
    int valid;
    do {
        valid = 1;
        food.y = rand() % (height - 2) + 1;
        food.x = rand() % (width - 2) + 1;
        for (int i = 0; i < snake_len; i++) {
            if (snake[i].x == food.x && snake[i].y == food.y) {
                valid = 0;
                break;
            }
        }
    } while (!valid);
}

void init_game() {
    getmaxyx(stdscr, height, width);
    if (height > 30) height = 30;
    if (width > 80) width = 80;

    snake_len = 3;
    int start_x = width / 2;
    int start_y = height / 2;

    for (int i = 0; i < snake_len; i++) {
        snake[i].x = start_x - i;
        snake[i].y = start_y;
    }

    dir = RIGHT;
    score = 0;
    game_over = 0;
    delay_us = INITIAL_DELAY;

    spawn_food();
}

void draw() {
    clear();

    // Bordas
    for (int x = 0; x < width; x++) {
        mvaddch(0, x, '#');
        mvaddch(height - 1, x, '#');
    }
    for (int y = 0; y < height; y++) {
        mvaddch(y, 0, '#');
        mvaddch(y, width - 1, '#');
    }

    // Comida
    attron(COLOR_PAIR(2));
    mvaddch(food.y, food.x, '@');
    attroff(COLOR_PAIR(2));

    // Cobra
    attron(COLOR_PAIR(1));
    mvaddch(snake[0].y, snake[0].x, 'O'); // cabeça
    for (int i = 1; i < snake_len; i++) {
        mvaddch(snake[i].y, snake[i].x, 'o');
    }
    attroff(COLOR_PAIR(1));

    // Placar
    mvprintw(0, 2, " Pontuacao: %d ", score);

    // Instrucoes
    mvprintw(height, 0, "Setas/WASD para mover | Q para sair");

    refresh();
}

void input() {
    int ch = getch();
    switch (ch) {
        case KEY_UP:
        case 'w':
        case 'W':
            if (dir != DOWN) dir = UP;
            break;
        case KEY_DOWN:
        case 's':
        case 'S':
            if (dir != UP) dir = DOWN;
            break;
        case KEY_LEFT:
        case 'a':
        case 'A':
            if (dir != RIGHT) dir = LEFT;
            break;
        case KEY_RIGHT:
        case 'd':
        case 'D':
            if (dir != LEFT) dir = RIGHT;
            break;
        case 'q':
        case 'Q':
            game_over = 1;
            break;
    }
}

void update() {
    Point new_head = snake[0];

    switch (dir) {
        case UP:    new_head.y--; break;
        case DOWN:  new_head.y++; break;
        case LEFT:  new_head.x--; break;
        case RIGHT: new_head.x++; break;
    }

    // Colisao com paredes
    if (new_head.x <= 0 || new_head.x >= width - 1 ||
        new_head.y <= 0 || new_head.y >= height - 1) {
        game_over = 1;
        return;
    }

    // Colisao com o proprio corpo
    for (int i = 0; i < snake_len; i++) {
        if (snake[i].x == new_head.x && snake[i].y == new_head.y) {
            game_over = 1;
            return;
        }
    }

    // Comeu a comida?
    int ate = (new_head.x == food.x && new_head.y == food.y);

    // Move o corpo (desloca cada segmento para a posicao do anterior)
    for (int i = snake_len; i > 0; i--) {
        snake[i] = snake[i - 1];
    }
    snake[0] = new_head;

    if (ate) {
        snake_len++;
        score += 10;
        if (delay_us > MIN_DELAY) {
            delay_us -= 3000; // aumenta a velocidade
        }
        spawn_food();
    }
}

void show_game_over() {
    clear();
    int cy = height / 2;
    int cx = width / 2;
    mvprintw(cy - 1, cx - 5, "GAME OVER");
    mvprintw(cy, cx - 9, "Pontuacao final: %d", score);
    mvprintw(cy + 2, cx - 14, "Pressione R para reiniciar ou Q para sair");
    refresh();

    nodelay(stdscr, FALSE); // espera input bloqueante
    int ch;
    while (1) {
        ch = getch();
        if (ch == 'r' || ch == 'R') {
            init_game();
            nodelay(stdscr, TRUE);
            return;
        } else if (ch == 'q' || ch == 'Q') {
            endwin();
            exit(0);
        }
    }
}

int main() {
    srand(time(NULL));

    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_GREEN, COLOR_BLACK);
        init_pair(2, COLOR_RED, COLOR_BLACK);
    }

    init_game();

    while (1) {
        input();
        if (game_over) {
            show_game_over();
            continue;
        }
        update();
        draw();
        usleep(delay_us);
    }

    endwin();
    return 0;
}