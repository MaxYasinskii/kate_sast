/*  This file is part of the Kate project.
 *
 *  SPDX-FileCopyrightText: 2017 Héctor Mesa Jiménez <hector@lcc.uma.es>
 *
 *  SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "codeanalysisselector.h"

#include "clazy.h"
#include "clazycurrent.h"
#include "clippy.h"
#include "cppcheck.h"
#include "eslint.h"
#include "flake8.h"
#include "ruff.h"
#include "shellcheck.h"
#include "tidy.h"

QStandardItemModel *KateProjectCodeAnalysisSelector::model(QObject *parent)
{
    auto model = new QStandardItemModel(parent);

    /*
     * available linters
     */
    const KateProjectCodeAnalysisTool *tools[] = {
        // cppcheck, for C++
        new KateProjectCodeAnalysisToolCppcheck(model),
        // flake8, for Python
        new KateProjectCodeAnalysisToolFlake8(model),
        // ruff (python)
        new RuffTool(model),
        // ShellCheck, for sh/bash scripts
        new KateProjectCodeAnalysisToolShellcheck(model),
        // clazy for Qt C++
        new KateProjectCodeAnalysisToolClazy(model),
        // clang-tidy
        new KateProjectCodeAnalysisToolClazyCurrent(model),
        // eslint-current-file
        new ESLint(model),
        // clippy
        new Clippy(model),
        // html tidy
        new HtmlTidyTool(model),
    };

    QList<QStandardItem *> column;

    for (auto tool : tools) {
        auto item = new QStandardItem(tool->name());
        item->setData(QVariant::fromValue(tool), Qt::UserRole + 1);

        column << item;
    }

    model->appendColumn(column);

    return model;
}
