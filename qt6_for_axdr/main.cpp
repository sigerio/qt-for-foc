#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include <QVBoxLayout>
#include <QFont>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 从资源文件加载阿里普惠体
    int fontId = QFontDatabase::addApplicationFont(":/fonts/fonts/AlibabaPuHuiTi-Regular.ttf");
    if (fontId != -1) {
        QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
        if (!fontFamilies.isEmpty()) {
            QFont font(fontFamilies.first(), 12);
            app.setFont(font);
        }
    }

    QMainWindow window;
    window.setWindowTitle("Qt6 For AXDR");
    window.resize(800, 600);

    // 创建中央部件
    QWidget *centralWidget = new QWidget(&window);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    QLabel *label = new QLabel("Qt6 环境搭建成功！");
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);

    window.setCentralWidget(centralWidget);
    window.show();

    return app.exec();
}
