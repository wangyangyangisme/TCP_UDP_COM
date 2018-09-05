#include <QtGui>
#include "mystyle.h"

myStyle::myStyle()
{
}

void myStyle::polish(QPalette &palette)
{
    QColor brown(212, 140, 95);//褐色
    QColor slightlyOpaqueBlack(0, 0, 0, 63);
    QColor veryLightGreen(204,232,215);//浅绿色

    QPixmap backgroundImage(":/images/bgPortrait.jpg");
    QPixmap buttonImage(":/images/wood.jpg");
    QPixmap midImage = buttonImage;//滑动条背景

    QPainter painter;//画笔
    painter.begin(&midImage);
    painter.setPen(Qt::NoPen);
    painter.fillRect(midImage.rect(), slightlyOpaqueBlack);
    painter.end();

    palette = QPalette(brown);//调色板

    palette.setBrush(QPalette::BrightText, Qt::white);
    palette.setBrush(QPalette::Base, veryLightGreen);//设置输入框背景
    palette.setBrush(QPalette::Highlight, Qt::darkGreen);//设置鼠标选中的背景色为深绿色
    palette.setBrush(QPalette::Button, buttonImage);//设置按钮背景
    palette.setBrush(QPalette::Mid, midImage);//设置滑动条背景
    palette.setBrush(QPalette::Window, backgroundImage);//设置应用程序背景


    QBrush brush = palette.background();
    brush.setColor(brush.color().dark());
//    QColor DisabledColor(65,105,225);//品蓝
//    brush.setColor(DisabledColor);

    palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush);
    palette.setBrush(QPalette::Disabled, QPalette::Text, brush);
    palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush);
    palette.setBrush(QPalette::Disabled, QPalette::Base, brush);
    palette.setBrush(QPalette::Disabled, QPalette::Button, brush);
    palette.setBrush(QPalette::Disabled, QPalette::Mid, brush);
}

void myStyle::polish(QWidget *widget)
{
    if (qobject_cast<QPushButton *>(widget)
            || qobject_cast<QComboBox *>(widget))
        widget->setAttribute(Qt::WA_Hover, true);
    //鼠标进入或者离开窗口部件所在区域时，会产生一个绘制事件
}

void myStyle::unpolish(QWidget *widget)
{
    if (qobject_cast<QPushButton *>(widget)
            || qobject_cast<QComboBox *>(widget))
        widget->setAttribute(Qt::WA_Hover, false);
}

int myStyle::pixelMetric(QStyle::PixelMetric metric,
                         const QStyleOption *option, const QWidget *widget) const
{
    switch (metric)
    {
    case PM_ComboBoxFrameWidth:
        return 6;
    case PM_ScrollBarExtent:
        return QWindowsStyle::pixelMetric(metric, option, widget) + 4;
    default:
        return QWindowsStyle::pixelMetric(metric, option, widget);
    }
}

int myStyle::styleHint(QStyle::StyleHint hint,
                       const QStyleOption *option, const QWidget *widget,
                       QStyleHintReturn *returnData) const
{
    switch (hint)
    {
    case SH_DitherDisabledText:
        return int(false);
    case SH_EtchDisabledText:
        return int(true);
    default:
        return QWindowsStyle::styleHint(hint, option, widget, returnData);
    }
}

void myStyle::drawPrimitive(QStyle::PrimitiveElement element,
                            const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{

    switch (element)
    {
    case PE_PanelButtonCommand:
        drawPushButton(option, painter);
        break;
    default:
        QWindowsStyle::drawPrimitive(element, option, painter, widget);
    }
}

void myStyle::drawControl(QStyle::ControlElement element,
                          const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    QWindowsStyle::drawControl(element, option, painter, widget);
}

void myStyle::drawPushButton(const QStyleOption *option, QPainter *painter) const
{
//    QColor buttonColor = option->palette.button().color();
    QColor buttonColor(255,128,0);//桔黄
//    QColor buttonColor(255,215,0);//金黄
    int coeff = (option->state & State_MouseOver) ? 115 : 105;

    QLinearGradient gradient(0, 0, 0, option->rect.height());//填充背景
    gradient.setColorAt(0.0, option->palette.light().color());
    gradient.setColorAt(0.2, buttonColor.lighter(coeff));
    gradient.setColorAt(0.8, buttonColor.darker(coeff));
    gradient.setColorAt(1.0, option->palette.dark().color());

    double penWidth = 1.0;
    if (const QStyleOptionButton *buttonOpt =
            qstyleoption_cast<const QStyleOptionButton *>(option)) {
        if (buttonOpt->features & QStyleOptionButton::DefaultButton)
            penWidth = 2.0;//默认按钮使用2像素边框
    }

    QRect roundRect = option->rect.adjusted(+1, +1, -1, -1);
    if (!roundRect.isValid())
        return;

    int diameter = 12;
    int cx = 100 * diameter / roundRect.width();//指定按钮圆角程度
    int cy = 100 * diameter / roundRect.height();

    painter->save();//执行绘图
    painter->setPen(Qt::NoPen);
    painter->setBrush(gradient);
    painter->drawRoundRect(roundRect, cx, cy);

    if (option->state & (State_On | State_Sunken))
    {//如果按钮被按下，添加一个75%透明的黑色效果
        QColor slightlyOpaqueBlack(0, 0, 0, 63);
        painter->setBrush(slightlyOpaqueBlack);
        painter->drawRoundRect(roundRect, cx, cy);
    }

    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(QPen(option->palette.foreground(), penWidth));
    painter->setBrush(Qt::NoBrush);
    painter->drawRoundRect(roundRect, cx, cy);
    painter->restore();
}
