#ifndef UI_COORD_TRANSFORM_PANEL_H
#define UI_COORD_TRANSFORM_PANEL_H

#include <QWidget>

class coord_transform_panel : public QWidget {
    Q_OBJECT
public:
    explicit coord_transform_panel(QWidget* parent = nullptr);
public slots:
    void update_display();
};

#endif // UI_COORD_TRANSFORM_PANEL_H
