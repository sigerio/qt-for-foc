#include <QApplication>
#include <QFont>
#include <QFontDatabase>
#include <QDebug>
#include "ui/foc_visualization_window.h"
#include "ui/control_loop_window.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("FOC可视化学习工具");

    // 从文件系统加载阿里普惠体
    QString font_path = QApplication::applicationDirPath() + "/../fonts/AlibabaPuHuiTi-Regular.ttf";
    int font_id = QFontDatabase::addApplicationFont(font_path);
    if (font_id != -1) {
        QStringList font_families = QFontDatabase::applicationFontFamilies(font_id);
        if (!font_families.isEmpty()) {
            QFont font(font_families.first(), 10);
            app.setFont(font);
        }
    } else {
        qDebug() << "字体加载失败，使用系统默认字体";
    }

    // 创建双窗口
    foc_visualization_window foc_win;
    control_loop_window ctrl_win;

    // 设置窗口位置
    foc_win.move(100, 100);
    ctrl_win.move(920, 100);

    // 显示窗口
    foc_win.show();
    ctrl_win.show();

    return app.exec();
}
