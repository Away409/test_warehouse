#include <graphics.h>
#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include <ctime>

// 游戏参数
const int WIN_W = 600;
const int WIN_H = 700;

// 挡板
const int PAD_W = 100;
const int PAD_H = 15;

// 小球
const int BALL_R = 8;

// 砖块
const int BRICK_ROW = 6;
const int BRICK_COL = 10;
const int BRICK_W = 55;
const int BRICK_H = 22;

// 砖块类
struct Brick {
    bool exist;
    int x, y;
    COLORREF color;
} bricks[BRICK_ROW][BRICK_COL];

// 初始化砖块
void initBricks() {
    int colors[] = { RED, BLUE, GREEN, YELLOW, CYAN, MAGENTA };
    for (int i = 0; i < BRICK_ROW; i++) {
        for (int j = 0; j < BRICK_COL; j++) {
            bricks[i][j].exist = true;
            bricks[i][j].x = j * BRICK_W + 25;
            bricks[i][j].y = i * BRICK_H + 50;
            bricks[i][j].color = colors[i % 6];
        }
    }
}

// 绘制所有砖块
void drawBricks() {
    for (int i = 0; i < BRICK_ROW; i++) {
        for (int j = 0; j < BRICK_COL; j++) {
            if (bricks[i][j].exist) {
                setfillcolor(bricks[i][j].color);
                solidrectangle(bricks[i][j].x, bricks[i][j].y,
                    bricks[i][j].x + BRICK_W - 1, bricks[i][j].y + BRICK_H - 1);
            }
        }
    }
}

int main() {
    initgraph(WIN_W, WIN_H);
    BeginBatchDraw();  // 双缓冲，不闪
    srand((unsigned)time(NULL));

    // 挡板位置
    int padX = WIN_W / 2 - PAD_W / 2;
    int padY = WIN_H - 50;

    // 小球
    int ballX = WIN_W / 2;
    int ballY = WIN_H / 2;
    int dx = (rand() % 2 == 0) ? 4 : -4;
    int dy = -4;

    // 分数
    int score = 0;
    bool gameOver = false;
    bool win = false;

    initBricks();

    while (true) {
        cleardevice();
        setbkcolor(RGB(20, 20, 30));

        // 退出
        if (GetAsyncKeyState(VK_ESCAPE)) break;

        // 挡板左右移动
        if (GetAsyncKeyState('A') && padX > 0) {
            padX -= 6;
        }
        if (GetAsyncKeyState('D') && padX + PAD_W < WIN_W) {
            padX += 6;
        }

        if (!gameOver && !win) {
            // 小球移动
            ballX += dx;
            ballY += dy;

            // 左右墙壁反弹
            if (ballX <= BALL_R || ballX >= WIN_W - BALL_R)
                dx = -dx;

            // 上墙壁反弹
            if (ballY <= BALL_R)
                dy = -dy;

            // 掉下去 = 失败
            if (ballY >= WIN_H - BALL_R) {
                gameOver = true;
            }

            // 挡板反弹
            if (ballY + BALL_R >= padY &&
                ballX >= padX && ballX <= padX + PAD_W &&
                dy > 0) {
                dy = -dy;
                // 左右偏移让角度变化
                dx = (ballX - (padX + PAD_W / 2)) / 10;
            }

            // 砖块碰撞
            for (int i = 0; i < BRICK_ROW; i++) {
                for (int j = 0; j < BRICK_COL; j++) {
                    if (bricks[i][j].exist) {
                        int bx = bricks[i][j].x;
                        int by = bricks[i][j].y;
                        if (ballX + BALL_R >= bx && ballX - BALL_R <= bx + BRICK_W &&
                            ballY + BALL_R >= by && ballY - BALL_R <= by + BRICK_H) {
                            dy = -dy;
                            bricks[i][j].exist = false;
                            score += 10;
                        }
                    }
                }
            }

            // 判断胜利
            win = true;
            for (int i = 0; i < BRICK_ROW; i++)
                for (int j = 0; j < BRICK_COL; j++)
                    if (bricks[i][j].exist) win = false;
        }

        // ========== 绘制 ==========
        // 砖块
        drawBricks();

        // 挡板
        setfillcolor(WHITE);
        solidrectangle(padX, padY, padX + PAD_W, padY + PAD_H);

        // 小球
        setfillcolor(RGB(255, 200, 0));
        solidcircle(ballX, ballY, BALL_R);

        // 分数
        setcolor(WHITE);
        wchar_t buf[30];
        swprintf_s(buf, L"分数：%d", score);
        outtextxy(10, 10, buf);

        // 结束文字
        if (gameOver) {
            setcolor(RED);
            outtextxy(WIN_W / 2 - 60, WIN_H / 2, L"游戏结束");
        }
        if (win) {
            setcolor(GREEN);
            outtextxy(WIN_W / 2 - 60, WIN_H / 2, L"恭喜通关！");
        }

        FlushBatchDraw();
        Sleep(12);
    }

    closegraph();
    return 0;
}