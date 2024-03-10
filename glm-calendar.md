如果你将`QCalendarWidget`封装在另一个自定义的widget中，并且想要在`MainWindow`中知道日历控件是否被关闭，你可以通过自定义信号来实现这一功能。在封装的widget中，当你检测到日历控件应该“关闭”的时候（这可能是因为用户选择了日期或者以其他方式交互），你可以发出一个自定义信号。然后在`MainWindow`中，你可以连接到这个信号来执行相应的操作。
以下是一个简单的例子，展示了如何实现这个功能：
首先，在你的自定义widget中：
```cpp
#ifndef CUSTOMCALENDARWIDGET_H
#define CUSTOMCALENDARWIDGET_H
#include <QWidget>
#include <QCalendarWidget>
#include <QSignalMapper>
class CustomCalendarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CustomCalendarWidget(QWidget *parent = nullptr);
    ~CustomCalendarWidget();
signals:
    // 自定义信号，当日期被选择或者日历被关闭时发出
    void dateSelected(const QDate &date);
private slots:
    void onDateSelected(const QDate &date);
private:
    QCalendarWidget *calendar;
    QSignalMapper *signalMapper;
};
#endif // CUSTOMCALENDARWIDGET_H
```
然后，在自定义widget的实现文件中：
```cpp
#include "customcalendarwidget.h"
CustomCalendarWidget::CustomCalendarWidget(QWidget *parent)
    : QWidget(parent)
{
    calendar = new QCalendarWidget(this);
    signalMapper = new QSignalMapper(this);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(calendar);
    // 将 calendar 的 clicked 信号连接到 signalMapper
    connect(calendar, &QCalendarWidget::clicked, signalMapper, static_cast<void(QSignalMapper::*)(const QString &)>(&QSignalMapper::map));
    // 设置 signalMapper 的映射
    signalMapper->setMapping(calendar, "calendar");
    // 连接 signalMapper 的 mapped 信号到我们的自定义槽
    connect(signalMapper, &QSignalMapper::mapped, this, &CustomCalendarWidget::onDateSelected);
}
CustomCalendarWidget::~CustomCalendarWidget()
{
}
void CustomCalendarWidget::onDateSelected(const QString &source)
{
    if (source == "calendar") {
        // 发出自定义信号，传递选中的日期
        emit dateSelected(calendar->selectedDate());
        // 这里你可以添加额外的逻辑来处理日历“关闭”的情况
    }
}
```
最后，在你的`MainWindow`中：
```cpp
#include "customcalendarwidget.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // ...
    CustomCalendarWidget *calendarWidget = new CustomCalendarWidget(this);
    // 连接自定义信号
    connect(calendarWidget, &CustomCalendarWidget::dateSelected, this, &MainWindow::onCalendarDateSelected);
    // ...
}
void MainWindow::onCalendarDateSelected(const QDate &date)
{
    // 处理选中的日期
    // 这里你可以添加逻辑来响应日历的“关闭”
}
```
在这个例子中，当用户在`CustomCalendarWidget`中选择了一个日期时，`CustomCalendarWidget`会发出`dateSelected`信号。`MainWindow`中的`onCalendarDateSelected`槽函数会被调用，你可以在这里添加逻辑来处理日历的“关闭”事件。注意，这里的“关闭”并不是指实际的窗口关闭，而是指日历的选择行为完成。如果你需要处理实际的窗口关闭事件，你可能需要在自定义widget中添加更多的逻辑来检测这一点。
