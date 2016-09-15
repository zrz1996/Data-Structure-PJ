#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    time_t t1 = clock();
    load_data();
    Graph::build();
    KDTW.build(Graph::P, 1, Graph::n + 1);
    KDTB.build(Graph::P1, 1, Graph::n1 + 1);
    SEGMENTTREE::init();
    TAXI::getIDlist();
    for (int i = 0; i < (int)TAXI::TaxiID.size(); i++)
        ui->LIST->insertItem(i, QString::number(TAXI::TaxiID[i]));
    connect(&MS, &Mysignal::valueChanged, this, &MainWindow::myslot);
    connect(&MS, &Mysignal::valueChanged1, this, &MainWindow::myslot1);
    srand(time(0));
    time_t t2 = clock();
    printf("Init: %.4f\n", (double)(t2 - t1) / CLOCKS_PER_SEC);
    fflush(stdout);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    draw_map(0);
}

void MainWindow::on_pushButton_2_clicked()
{
    int n = Graph::n;
    int u = rand() % n + 1, v = rand() % n + 1;
    QString s;
    clock_t t1, t2;
    double t, d;
    s = "随机选定的起、终点：" + QString::number(Node[Graph::relabel[u]].id) + " " + QString::number(Node[Graph::relabel[v]].id) + "\n";
    t1 = clock();
    d = Dijkstra::query(u, v);
    t2 = clock();
    t = (double)(t2 - t1) / CLOCKS_PER_SEC * 1000;
    s += "Dijkstra完成，耗时" + QString::number(t, 'f') + "ms\n";
    s += "dis = " + QString::number(d, 'f') + "\n";
    t1 = clock();
    d = Astar::query(u, v);
    t2 = clock();
    t = (double)(t2 - t1) / CLOCKS_PER_SEC * 1000;
    s += "A*完成，耗时" + QString::number(t, 'f') + "ms\n";
    s += "dis = " + QString::number(d, 'f') + "\n";
    t1 = clock();
    d = SPFA::query(u, v);
    t2 = clock();
    t = (double)(t2 - t1) / CLOCKS_PER_SEC * 1000;
    s += "SPFA完成，耗时" + QString::number(t, 'f') + "ms\n";
    s += "dis = " + QString::number(d, 'f') + "\n";
    ui->OUT->setPlainText(s);
    getpath(u, v);
}

void MainWindow::on_pushButton_3_clicked()
{
    draw_map(1);
    draw_reset();
}
extern point_t p1, p2;
void MainWindow::on_pushButton_4_clicked()
{
    draw_map(2);
    KDTW.query(p1, 2);
    int s[2];
    s[0] = KDTW.kans[0], s[1] = KDTW.kans[1];
    KDTW.query(p2, 2);
    int t[2];
    t[0] = KDTW.kans[0], t[1] = KDTW.kans[1];
    QString str;
    str = "可能的起点：" + QString::number(Node[s[0]].id) + " " + QString::number(Node[s[1]].id) + "\n";
    str += "可能的终点：" + QString::number(Node[t[0]].id) + " " + QString::number(Node[t[1]].id) + "\n";
    double d = 1e9;
    int ss, tt;
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
        {
            int u = Graph::NodeFilter[s[i]], v = Graph::NodeFilter[t[j]];
            double d1 = Astar::query(u, v);
            if (d1 < d)
            {
                d = d1;
                getpath(u, v);
                ss = s[i], tt = t[j];
            }
        }
    str += "选定的起、终点：" + QString::number(Node[ss].id) + " " + QString::number(Node[tt].id) + "\n";
    str += "最短距离：" + QString::number(d) + "米\n";
    ui->OUT->setPlainText(str);
    draw_map(3);
    draw_reset();
}


void MainWindow::on_pushButton_5_clicked()
{
    draw_map(2);
    clock_t t1, t2;
    double t;
    t1 = clock();
    SEGMENTTREE::query(p1, p2);
    t2 = clock();
    t = (double)(t2 - t1) / CLOCKS_PER_SEC * 1000;
    QString str = "线段树计算矩形内点数为：" + QString::number(SEGMENTTREE::vec.size()) + "\n";
    str += "线段树耗时："+ QString::number(t, 'f') + "ms\n";
    t1 = clock();
    KDTB.query(p1, p2);
    t2 = clock();
    t = (double)(t2 - t1) / CLOCKS_PER_SEC * 1000;
    str += "kd-tree计算矩形内点数为：" + QString::number(KDTB.vec.size()) + "\n";
    str += "kd-tree耗时："+ QString::number(t, 'f') + "ms\n";
    KDTB.getPointData(0);
    str += "矩形内共有道路" + QString::number(KDTB.kansWay.size()) + "条\n";
    str += "矩形内共有建筑" + QString::number(KDTB.kansBuilding.size()) + "座\n";
    ui->OUT->setPlainText(str);
    draw_reset();
}

void MainWindow::on_pushButton_6_clicked()
{
    unsigned id = ui->IN1->toPlainText().toUInt();
    if (id == 0)
        draw_map(2);
    else
    {
        if (labelNode.find(id) == labelNode.end())
        {
            ui->OUT->setPlainText("Point not found!\n");
            return;
        }
        int pid = labelNode[id];
        p1.x = Node[pid].x, pt1.y = p1.y = Node[pid].y;
        pt1.x = p1.x, pt1.y = p1.y;
    }
    int k = ui->IN->toPlainText().toInt();
    KDTB.query(p1, k * 10);
    KDTB.getPointData(1);
    QString s = "k近邻内共有道路" + QString::number(KDTB.kansWay.size()) + "条，分别是：\n";
    for (unordered_set<int>::iterator it = KDTB.kansWay.begin(); it != KDTB.kansWay.end(); it++)
    {
        int pid = *it;
        s += QString::number(Way[pid].id) + " " + QString(Way[pid].name) + "\n";
    }
    s += "k近邻内共有建筑" + QString::number(KDTB.kansBuilding.size()) + "座，分别是：\n";
    for (unordered_set<int>::iterator it = KDTB.kansBuilding.begin(); it != KDTB.kansBuilding.end(); it++)
    {
        int pid = *it;
        s += QString::number(Building[pid].id) + " " + QString(Building[pid].name) + "\n";
    }
    ui->OUT->setPlainText(s);
    draw_map(4);
    draw_reset();
}

void MainWindow::on_pushButton_7_clicked()
{
    int n = Graph::n;
    QString s;
    clock_t t1, t2;
    double th = 0, ta = 0;
    int u = rand() % n + 1, v = rand() % n + 1;
    int k = rand() % 1000;
    s = QString::number(Graph::relabel[u]) + " " + QString::number(Graph::relabel[v]) + " " + QString::number(k) + "\n";
    t1 = clock();
    double d = TOPKHEAP::query(u, v, k);
    t2 = clock();
    th += (double)(t2 - t1) / CLOCKS_PER_SEC * 1000;
    s += "top-k完成，耗时" + QString::number(th, 'f') + "ms\n";
    s += "dis = " + QString::number(d, 'f') + "\n";
    t1 = clock();
    d = TOPKASTAR::query(u, v, k);
    t2 = clock();
    ta += (double)(t2 - t1) / CLOCKS_PER_SEC * 1000;
    s += "top-k-A*完成，耗时" + QString::number(ta, 'f') + "ms\n";
    s += "dis = " + QString::number(d, 'f') + "\n";
    ui->OUT->setPlainText(s);
}

void MainWindow::on_pushButton_8_clicked()
{
    int id = ui->IN->toPlainText().toInt();
    if (id == 0)
    {
        TAXI::parserAll();
        ui->OUT->setPlainText("parser-all compelete!\n");
        draw_map(8);
        return;
    }
    QString s;
    if (!TAXI::parser(id))
    {
        s = "taxi id not found!\n";
        ui->OUT->setPlainText(s);
        return;
    }
    s = "编号：" + QString::number(id) + "\n";
    pair<int, int> t = TAXI::getTime(id);
    s += "共采样" + QString::number((int)TAXI::Taxi[id].size()) + "个点，采样时间" + QString::number(t.first) + " s\n";
    s += "载客时间" + QString::number(t.second) + "s 空车时间" + QString::number(t.first - t.second) + "s\n载客时间比例" + QString::number((double)t.second / t.first * 100.0, 'f', 2) + "%\n";
    ui->OUT->setPlainText(s);
    TAXI::getpath(id);
    draw_map(5);
}

void MainWindow::on_LIST_itemClicked(QListWidgetItem *item)
{
    bool ok;
    int id = item->text().toInt(&ok, 10);
    TAXI::parser(id);
    QString s = "编号：" + QString::number(id) + "\n";
    pair<int, int> t = TAXI::getTime(id);
    s += "共采样" + QString::number((int)TAXI::Taxi[id].size()) + "个点，采样时间" + QString::number(t.first) + " s\n";
    s += "载客时间" + QString::number(t.second) + "s 空车时间" + QString::number(t.first - t.second) + "s\n载客时间比例" + QString::number((double)t.second / t.first * 100.0, 'f', 2) + "%\n";
    ui->OUT->setPlainText(s);
}

void MainWindow::on_LIST_itemDoubleClicked(QListWidgetItem *item)
{
    bool ok;
    int id = item->text().toInt(&ok, 10);
    TAXI::getpath(id);
    draw_map(5);
}
void MainWindow::on_pushButton_9_clicked()
{
    QString name = ui->IN->toPlainText();
    ui->LIST1->clear();
    int num = 0;
    FIND::find(name.toStdString());
    for (int i = 0; i < (int)FIND::foundWay.size(); i++)
        ui->LIST1->insertItem(num++, Way[FIND::foundWay[i]].name);
    for (int i = 0; i < (int)FIND::foundBuilding.size(); i++)
        ui->LIST1->insertItem(num++, Building[FIND::foundBuilding[i]].name);
}

void MainWindow::on_LIST1_itemClicked()
{
    int currentRow = ui->LIST1->currentRow();
    QString s;
    if (currentRow < (int)FIND::foundWay.size())
    {
        s = "道路名称：";
        s += ui->LIST1->currentItem()->text() + "\n";
        int pid = FIND::foundWay[currentRow];
        s += "道路id：" + QString::number(Way[pid].id) + "\n";
        s += "道路长度：" + QString::number(Way[pid].len) + "\n";
    }
    else
    {
        currentRow -= FIND::foundWay.size();
        s = "建筑名称：";
        s += ui->LIST1->currentItem()->text() + "\n";
        int pid = FIND::foundBuilding[currentRow];
        s += "建筑id：" + QString::number(Building[pid].id) + "\n";
    }
    ui->OUT->setPlainText(s);
}

void MainWindow::on_LIST1_itemDoubleClicked()
{
    int row = ui->LIST1->currentRow();
    if (row < (int)FIND::foundWay.size())
    {
        FIND::cur = row;
        draw_map(6);
    }
    else
    {
        FIND::cur = row - (int)FIND::foundWay.size();
        draw_map(7);
    }
}

void MainWindow::myslot(int type, int id)
{
    QString s;
    if (type == 0)
    {
        s = "道路名称：";
        s += QString(Way[id].name) + "\n";
        s += "道路id：" + QString::number(Way[id].id) + "\n";
        s += "道路长度：" + QString::number(Way[id].len) + "\n";

    }
    else
    {
        s = "建筑名称：";
        s += QString(Building[id].name) + "\n";
        s += "建筑id：" + QString::number(Building[id].id) + "\n";
    }
    ui->OUT->setPlainText(s);
}
vector<pair<int, int> > Data;
void MainWindow::on_pushButton_16_clicked()
{
    Data.clear();
    int n = Graph::n;
    int task = ui->IN->toPlainText().toInt();
    while (task--)
    {
        int u = rand() % n + 1, v = rand() % n + 1;
        Data.push_back(make_pair(u, v));
    }
}
void MainWindow::on_pushButton_12_clicked()
{
    QString s;
    clock_t t1, t2;
    double t = 0;
    for (auto i : Data)
    {
        int u = i.first, v = i.second;
        t1 = clock();
        Dijkstra::query(u, v);
        t2 = clock();
        t += t2 - t1;
    }
    t = t / CLOCKS_PER_SEC * 1000;
    s += "Dijkstra完成，耗时" + QString::number(t, 'f') + "ms\n";
    ui->OUT->setPlainText(s);
}

void MainWindow::on_pushButton_10_clicked()
{
    QString s;
    clock_t t1, t2;
    double t = 0;
    for (auto i : Data)
    {
        int u = i.first, v = i.second;
        t1 = clock();
        Astar::query(u, v);
        t2 = clock();
        t += t2 - t1;
    }
    t = t / CLOCKS_PER_SEC * 1000;
    s += "A*完成，耗时" + QString::number(t, 'f') + "ms\n";
    ui->OUT->setPlainText(s);
}

void MainWindow::on_pushButton_11_clicked()
{
    QString s;
    clock_t t1, t2;
    double t = 0;
    for (auto i : Data)
    {
        int u = i.first, v = i.second;
        t1 = clock();
        SPFA::query(u, v);
        t2 = clock();
        t += t2 - t1;
    }
    t = t / CLOCKS_PER_SEC * 1000;
    s += "SPFA完成，耗时" + QString::number(t, 'f') + "ms\n";
    ui->OUT->setPlainText(s);
}

void MainWindow::on_pushButton_13_clicked()
{
    int task = ui->IN->toPlainText().toInt();
    clock_t t1, t2;
    double tk = 0, ts = 0, ts1 = 0;
    while (task--)
    {
        int x1 = rand() % 1000 + 1, x2 = x1 + 10, y1 = rand() % 1000 + 1, y2 = y1 + 10;
        if (x1 > x2)
            swap(x1, x2);
        if (y1 > y2)
            swap(y1, y2);
        point_t p1 = point_t(x1, y1), p2 = point_t(x2, y2);
        t1 = clock();
        KDTB.query(p1, p2);
        t2 = clock();
        tk += t2 - t1;
        t1 = clock();
        SEGMENTTREE::query(p1, p2);
        t2 = clock();
        ts += t2 - t1;
        /*
        t1 = clock();
        SEGMENTTREE1::query(p1, p2);
        t2 = clock();
        ts1 += t2 - t1;
        */
    }
    tk = tk / CLOCKS_PER_SEC * 1000, ts = ts / CLOCKS_PER_SEC * 1000;
    ts1 = ts1 / CLOCKS_PER_SEC * 1000;
    QString s = QString::number(task) + "组随机数据\nk-d树运行时间：" + QString::number(tk, 'f') + "ms\n线段树运行时间：" + QString::number(ts, 'f') + "ms\n";
    ui->OUT->setPlainText(s);
}

void MainWindow::on_pushButton_14_clicked()
{
    int task = 1000;
    clock_t t1, t2;
    double tk = 0, ts = 0;
    for (int i = 1; i <= task; i++)
    {
        double eps1 = (double)i / task + double(rand() % task) / task;
        double eps2 = (double)i / task + double(rand() % task) / task;
        point_t p = point_t(eps1, eps2);
        t1 = clock();
        KDTB.insert(p);
        t2 = clock();
        ts += t2 - t1;
    }
    task *= 10;
    while (task--)
    {
        int x2 = rand() % 1000 + 1, y2 = rand() % 1000 + 1;
        double eps1 = double(rand() % 1000) / 1000;
        double eps2 = double(rand() % 1000) / 1000;
        point_t p1 = point_t(eps1, eps2), p2 = point_t(x2, y2);
        t1 = clock();
        KDTB.query(p1, p2);
        t2 = clock();
        tk += t2 - t1;
    }
    tk = tk / CLOCKS_PER_SEC * 1000, ts = ts / CLOCKS_PER_SEC * 1000;
    QString s = "1000组插入数据，10000组查询数据\nk-d树查询时间：" + QString::number(tk, 'f') + "ms\nk-d树插入时间：" + QString::number(ts, 'f') + "ms\n";
    ui->OUT->setPlainText(s);
}

void MainWindow::on_pushButton_15_clicked()
{
    if (TAXI::TaxiP.size() == 0)
        TAXI::parserAll();
    draw_map(9);
}
void MainWindow::myslot1(double d)
{
    QString s = "打车指数：" + QString::number(d / 20, 'f', 2) + "%\n";
    ui->OUT->setPlainText(s);
}

