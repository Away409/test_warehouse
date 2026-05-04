#include <graphics.h>
#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include <ctime>

const int ROWS = 40;    // 行数
const int COLS = 60;    // 列数
const int CELL = 15;    // 每个细胞像素大小

int delayTime=100;      //第三次修改新增的演化间隔，这个值越小演化越快

class CellGame {
private:
    bool curr[ROWS][COLS];
    bool next[ROWS][COLS];

    int getNeighborCount(int x, int y) {
        int cnt = 0;
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (dx == 0 && dy == 0) continue;
                int nx = x + dx;
                int ny = y + dy;
                if (nx >= 0 && nx < ROWS && ny >= 0 && ny < COLS) {
                    if (curr[nx][ny]) cnt++;
                }
            }
        }
        return cnt;
    }

public:
    void init() {
        srand((unsigned)time(NULL));
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                curr[i][j] = (rand() % 100 < 15);
            }
        }
    }

    void clear() {
        for (int i = 0; i < ROWS; i++)
            for (int j = 0; j < COLS; j++)
                curr[i][j] = false;
    }

    void setCell(int x, int y, bool val) {
        if (x >= 0 && x < ROWS && y >= 0 && y < COLS)
            curr[x][y] = val;
    }

    void update() {
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                int num = getNeighborCount(i, j);
                if (curr[i][j]) {
                    next[i][j] = (num == 2 || num == 3);
                }
                else {
                    next[i][j] = (num == 3);
                }
            }
        }
        for (int i = 0; i < ROWS; i++)
            for (int j = 0; j < COLS; j++)
                curr[i][j] = next[i][j];
    }

    void draw() {
        // 只在开始时清一次背景，后续在双缓冲内绘制
        cleardevice();

        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                int x = j * CELL;
                int y = i * CELL;
                if (curr[i][j]) {
                    setfillcolor(RGB(100, 200, 255));
                    solidrectangle(x, y, x + CELL - 1, y + CELL - 1);
                }
                else {
                    setfillcolor(RGB(40, 40, 60));
                    solidrectangle(x, y, x + CELL - 1, y + CELL - 1);
                }
            }
        }

        setcolor(WHITE);
        outtextxy(10, ROWS * CELL + 5, L"鼠标左键：画细胞 | 右键：清除 | 空格：开始/暂停 | C：清空");
    }
};

int main() {
    initgraph(COLS * CELL, ROWS * CELL + 30);
    // 开启双缓冲防闪烁（关键！）
    BeginBatchDraw();

    CellGame game;
    game.init();

    bool isRun = false;

    bool spacePressed = false;  // 空格防抖
    bool cPressed = false;      // C键防抖
    //这两行为第二次新增的键盘防抖变量

    while (true) {
        // 按键检测

        // 上键加速、下键减速(新增功能，使操作时更为方便)
        if (GetAsyncKeyState(VK_UP) & 0x8000)
        {
            if(delayTime > 20) delayTime -= 10;
        }
        if (GetAsyncKeyState(VK_DOWN) & 0x8000)
        {
            if(delayTime < 500) delayTime += 10;
        }

        // 空格：开始/暂停
        if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
            if (!spacePressed) {
                isRun = !isRun;
                spacePressed = true;
            }
        } else {
            spacePressed = false;
        }

        // C键：清空画布
        if (GetAsyncKeyState('C') & 0x8000) {
            if (!cPressed) {
                game.clear();
                isRun = false;
                cPressed = true;
            }
        } else {
            cPressed = false;
        }//第二次改动后的键盘检测逻辑

        // 鼠标操作
        if (MouseHit()) {
            MOUSEMSG m = GetMouseMsg();
            int x = m.y / CELL;
            int y = m.x / CELL;
            if (m.uMsg == WM_LBUTTONDOWN) game.setCell(x, y, true);
            if (m.uMsg == WM_RBUTTONDOWN) game.setCell(x, y, false);
        }

        // 运行时更新
        if (isRun) {
            game.update();
            Sleep(delayTime);       //将括弧里面的常数100修改为delayTime
        }

        // 绘制在离屏缓冲区
        game.draw();
        // 一次性把画面刷新到屏幕上（关键！）
        FlushBatchDraw();
    }

    closegraph();
    return 0;
}