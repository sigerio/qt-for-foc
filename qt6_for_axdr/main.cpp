/**
 * @file main.cpp
 * @brief AxDr电机控制上位机主程序入口
 */

#include <QApplication>
#include <QFont>
#include <QFontDatabase>
#include <QDir>
#include <QDebug>

#include "ui/main_window.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    /* 从文件系统加载阿里普惠体（避免资源文件过大导致编译内存不足） */
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

    /* 创建并显示主窗口 */
    main_window_t window;
    window.show();

    return app.exec();
}
