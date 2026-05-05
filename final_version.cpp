#include <graphics.h>
#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include <ctime>

const int ROWS = 40;    // 行数
const int COLS = 60;    // 列数
const int CELL = 15;    // 每个细胞像素大小

int delayTime = 100;      //第三次修改新增的演化间隔，这个值越小演化越快
bool isRun = false;     //第四次将其修改为全局变量

bool isPause = false;
int liveCount = 0;      //第六次新增

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

    //第六次增加的统计函数
    int getLiveCount()
    {
        int cnt = 0;
        for (int i = 0; i < ROWS; i++)
        {
            for (int j = 0; j < COLS; j++)
            {
                if (curr[i][j]) cnt++;
            }
        }
        return cnt;
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

        // 绘制网格线（第四次修改新增，提升视觉效果）
        setcolor(RGB(60, 60, 80));
        // 横线
        for (int i = 0; i <= ROWS; i++)
        {
            line(0, i * CELL, COLS * CELL, i * CELL);
        }
        // 竖线
        for (int j = 0; j <= COLS; j++)
        {
            line(j * CELL, 0, j * CELL, ROWS * CELL);
        }



        setcolor(WHITE);
        //outtextxy(10, ROWS * CELL + 5, L"鼠标左键：画细胞 | 右键：清除 | 空格：开始/暂停 | C：清空");
        outtextxy(5, ROWS * CELL + 5, L"左键：画 | 右键：擦 | 空格：启停 | C：清空");
        outtextxy(5, ROWS * CELL + 25, L"↑↓：调速 | R：重置 | ESC：退出");

        // 状态与速度显示（第四次新增）
        wchar_t buf[64];
        swprintf(buf, 64, L"状态：%s  演化间隔：%dms", isRun ? L"运行中" : L"已暂停", delayTime);
        outtextxy(200, ROWS * CELL + 5, buf);

        // 第六次新增：显示存活细胞数
        liveCount = this->getLiveCount();
        swprintf(buf, 64, L"存活细胞：%d", liveCount);
        outtextxy(COLS * CELL - 110, 8, buf);

        // 第六次新增：暂停状态文字提示
        if (isPause)
        {
            setcolor(RGB(255, 200, 0));
            outtextxy(COLS * CELL / 2 - 75, 8, L"已暂停 回车单步");
        }
    }
};



//第五次新增控制开始/结束界面的函数
void showStartUI()
{
    cleardevice();
    // 深蓝背景
    setbkcolor(RGB(15, 25, 45));
    cleardevice();

    setcolor(WHITE);
    settextstyle(36, 0, L"微软雅黑");
    outtextxy(180, 160, L"生命游戏");
    settextstyle(20, 0, L"微软雅黑");
    outtextxy(160, 230, L"按 回车键 开始游戏");

    // 第六次新增：开始界面操作提示
    settextstyle(18, 0, L"微软雅黑");
    outtextxy(150, 270, L"空格暂停 | 暂停时回车单步演化");

    FlushBatchDraw();

    // 等待回车键按下
    while (1)
    {
        if (GetAsyncKeyState(VK_RETURN) & 0x8000)
        {
            break;
        }
        Sleep(10);
    }
}

void showEndUI()
{
    cleardevice();

    // 红黑渐变背景
    for (int y = 0; y < 600; y++)
    {
        setcolor(RGB(100, 20, 20));
        line(0, y, 1000, y);
    }

    // 白色大字
    setcolor(WHITE);
    settextstyle(40, 0, L"微软雅黑");
    outtextxy(200, 200, L"游戏结束");
    settextstyle(24, 0, L"微软雅黑");
    outtextxy(180, 280, L"按 ESC 退出");

    // 强制刷新
    FlushBatchDraw();

    // 按 ESC 才退出
    while (1)
    {
        if (GetAsyncKeyState(VK_ESCAPE) & 1)
        {
            break;
        }
        Sleep(50);
    }

    // 直接退出程序
    exit(0);
}


int main() {
    initgraph(COLS * CELL, ROWS * CELL + 50);
    // 开启双缓冲防闪烁
    BeginBatchDraw();
    showStartUI();

    CellGame game;
    game.init();



    bool spacePressed = false;  // 空格防抖
    bool cPressed = false;      // C键防抖
    //这两行为第二次新增的键盘防抖变量

    while (true) {
        // 按键检测

        // 上键加速、下键减速(新增功能，使操作时更为方便)
        if (GetAsyncKeyState(VK_UP) & 0x8000)
        {
            if (delayTime > 20) delayTime -= 10;
        }
        if (GetAsyncKeyState(VK_DOWN) & 0x8000)
        {
            if (delayTime < 500) delayTime += 10;
        }

        // 第六次新增：空格切换暂停状态
        if (GetAsyncKeyState(VK_SPACE) & 0x8000)
        {
            if (!spacePressed)
            {
                isPause = !isPause;
                // 暂停时停止自动演化，恢复时继续自动演化
                isRun = !isPause;
                spacePressed = true;
            }
        }
        else
        {
            spacePressed = false;
        }

        // 第六次新增：暂停状态下按回车单步演化
        if (isPause)
        {
            if (GetAsyncKeyState(VK_RETURN) & 0x8000)
            {
                game.update();
                Sleep(180);
            }
        }

        // C键：清空画布
        if (GetAsyncKeyState('C') & 0x8000) {
            if (!cPressed) {
                game.clear();
                isRun = false;
                isPause = false;   // 第六次新增：清空同时取消暂停
                cPressed = true;
            }
        }
        else {
            cPressed = false;
        }//第二次改动后的键盘检测逻辑

        // R键 随机重置地图（第五次新增）
        if (GetAsyncKeyState('R') & 0x8000)
        {
            game.init();
            isRun = false;
            isPause = false;   // 第六次新增：重置同时取消暂停
        }

        // ESC 退出（第五次新增）
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
        {
            showEndUI();  // 调用后会卡死在这里
            //break;
        }


        // 鼠标操作(第五次修改后支持按住鼠标左键连续创建细胞，按住右键连续消除细胞)
        if (MouseHit())
        {
            MOUSEMSG m = GetMouseMsg();
            int gx = m.y / CELL;
            int gy = m.x / CELL;

            if (m.uMsg == WM_LBUTTONDOWN || m.uMsg == WM_MOUSEMOVE && (GetAsyncKeyState(VK_LBUTTON) & 0x8000))
            {
                game.setCell(gx, gy, true);
            }
            if (m.uMsg == WM_RBUTTONDOWN || m.uMsg == WM_MOUSEMOVE && (GetAsyncKeyState(VK_RBUTTON) & 0x8000))
            {
                game.setCell(gx,gy, false);
            }
        }

        // 运行时更新
        if (isRun && !isPause) {
            game.update();
            Sleep(delayTime);       //将括弧里面的常数100修改为delayTime
        }

        // 绘制在离屏缓冲区
        game.draw();
        // 一次性把画面刷新到屏幕上
        FlushBatchDraw();
    }

    closegraph();
    return 0;
}