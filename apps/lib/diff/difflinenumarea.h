/*
    SPDX-FileCopyrightText: 2022 Waqar Ahmed <waqar.17a@gmail.com>
    SPDX-License-Identifier: LGPL-2.0-or-later
*/
#ifndef DIFF_LINE_NUM_AREA
#define DIFF_LINE_NUM_AREA

#include <QWidget>

class LineNumArea final : public QWidget
{
    Q_OBJECT
public:
    explicit LineNumArea(class DiffEditor *parent);

    int lineNumAreaWidth() const;
    QSize sizeHint() const override;

    void setLineNumData(QVector<int>);
    void setMaxLineNum(int n)
    {
        maxLineNum = n;
    }

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *e) override;

private:
    class DiffEditor *const textEdit;
    QColor m_currentLineColor;
    QColor m_otherLinesColor;
    QColor m_borderColor;
    QVector<int> m_lineToNum;
    int maxLineNum = 0;
};

#endif