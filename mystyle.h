#ifndef MYSTYLE_H
#define MYSTYLE_H

#include <QPalette>
#include <QWindowsStyle>
#include <QMotifStyle>

class myStyle : public QWindowsStyle
{
    Q_OBJECT
public:
    myStyle();
//以下三个函数在装载Style和卸载Style时调用,它可以是适当修改窗口部件和调色板
    void polish(QPalette &palette);// 改变调色板为样式指定的颜色调色板
    void polish(QWidget *widget);//初始化给定窗口部件的外观,窗口部件每一次创建后首次显示之前调用
    void unpolish(QWidget *widget);//取消polish()的作用
    int pixelMetric(PixelMetric metric, const QStyleOption *option,
                    const QWidget *widget) const;//设置各部件的线宽，大小等
    int styleHint(StyleHint hint, const QStyleOption *option,
                  const QWidget *widget, QStyleHintReturn *returnData) const;
    //分别设置各种部件的布局，比如checkbox的text在左边或者在右边

    void drawPrimitive(PrimitiveElement element, const QStyleOption *option,
                       QPainter *painter, const QWidget *widget) const;
    //这个则是最重要的，在这里面可以实现对不同部件的原始元素的绘制，当然你也可以调用默认的方法.
   // element这个参数就是我们需要重绘的部件的原始元素

    void drawControl(ControlElement element, const QStyleOption *option,
                     QPainter *painter, const QWidget *widget) const;

private:
    void drawPushButton(const QStyleOption *option,
                         QPainter *painter) const;
};

#endif // MYSTYLE_H
