/* This file is part of the KDE libraries
   Copyright (C) 2005-2006 Hamish Rodda <rodda@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef KDELIBS_KTEXTEDITOR_CODECOMPLETIONMODEL_H
#define KDELIBS_KTEXTEDITOR_CODECOMPLETIONMODEL_H

#include <ktexteditor/ktexteditor_export.h>
#include <QtCore/QModelIndex>
#include <ktexteditor/range.h>

namespace KTextEditor {

class Document;
class View;

/**
 * \short An item model for providing code completion, and meta information for
 *        enhanced presentation.
 *
 * \todo write documentation
 *
 * @author Hamish Rodda <rodda@kde.org>
 */
class KTEXTEDITOR_EXPORT CodeCompletionModel : public QAbstractItemModel
{
  Q_OBJECT

  public:
    CodeCompletionModel(QObject* parent);
    virtual ~CodeCompletionModel();

    enum Columns {
      Prefix = 0,
      /// Icon representing the type of completion. We have a separate icon field
      /// so that names remain aligned where only some completions have icons,
      /// and so that they can be rearranged by the user.
      Icon,
      Scope,
      Name,
      Arguments,
      Postfix
    };
    static const int ColumnCount = Postfix + 1;

    enum CompletionProperty {
      NoProperty  = 0x0,
      FirstProperty = 0x1,

      // Access specifiers - no more than 1 per item
      Public      = 0x1,
      Protected   = 0x2,
      Private     = 0x4,

      // Extra access specifiers - any number per item
      Static      = 0x8,
      Const       = 0x10,

      // Type - no more than 1 per item (except for Template)
      Namespace   = 0x20,
      Class       = 0x40,
      Struct      = 0x80,
      Union       = 0x100,
      Function    = 0x200,
      Variable    = 0x400,
      Enum        = 0x800,
      Template    = 0x1000,

      // Special attributes - any number per item
      Virtual     = 0x2000,
      Override    = 0x4000,
      Inline      = 0x8000,
      Friend      = 0x10000,
      Signal      = 0x20000,
      Slot        = 0x40000,

      // Scope - no more than 1 per item
      LocalScope      = 0x80000,
      NamespaceScope  = 0x100000,
      GlobalScope     = 0x200000,

      // Keep this in sync so the code knows when to stop
      LastProperty    = GlobalScope
    };
    Q_DECLARE_FLAGS(CompletionProperties, CompletionProperty)

    enum HighlightMethod {
      NoHighlighting        = 0x0,
      InternalHighlighting  = 0x1,
      CustomHighlighting    = 0x2
    };
    Q_DECLARE_FLAGS(HighlightMethods, HighlightMethod)

    /// Meta information is passed through extra {Qt::ItemDataRole}s.
    /// This information should be returned when requested on the Name column.
    enum ExtraItemDataRoles {
      /// The model should return a set of CompletionProperties.
      CompletionRole = Qt::UserRole,

      /// The model should return an index to the scope
      /// -1 represents no scope
      /// \todo how to sort scope?
      ScopeIndex,

      /**
       * If requested, your model should try to determine whether the
       * completion in question is a suitable match for the context (ie.
       * is accessible, exported, + returns the data type required)
       *
       * Return a bool, \e true if the completion is suitable, otherwise
       * return \e false.  Return QVariant::Invalid if you are unable to
       * determine this.
       */
      MatchType,

      /**
       * Define which highlighting method will be used:
       * - QVariant::Invalid - allows the editor to choose (usually internal highlighting)
       * - QVariant::Integer - highlight as specified by HighlightMethods.
       */
      HighlightingMethod,

      /**
       * Allows an item to provide custom highlighting.  Return a
       * QList\<QVariant\> in the following format:
       * - int startColumn (where 0 = start of the completion entry)
       * - int endColumn (note: not length)
       * - QTextFormat attribute (note: attribute may be a KTextEditor::Attribute, as it is a child class)
       *
       * Repeat this triplet as many times as required, however each column must be >= the previous,
       * and startColumn != endColumn.
       */
      CustomHighlight,

      /**
       * Returns the inheritance depth of the completion.  For example, a completion
       * which comes from the base class would have depth 0, one from a parent class
       * would have depth 1, one from that class' parent 2, etc.
       */
      InheritanceDepth
    };
    static const int LastItemDataRole = InheritanceDepth;

    void setRowCount(int rowCount);

    enum InvocationType {
      AutomaticInvocation,
      UserInvocation,
      ManualInvocation
    };

    virtual void completionInvoked(KTextEditor::View* view, const KTextEditor::Range& range, InvocationType invocationType);
    virtual void executeCompletionItem(Document* document, const Range& word, int row) const;

    // Reimplementations
    virtual int columnCount ( const QModelIndex & parent = QModelIndex() ) const;
    virtual QModelIndex index ( int row, int column, const QModelIndex & parent = QModelIndex() ) const;
    virtual QMap<int, QVariant> itemData ( const QModelIndex & index ) const;
    virtual QModelIndex parent ( const QModelIndex & index ) const;
    virtual int rowCount ( const QModelIndex & parent = QModelIndex() ) const;

  private:
    class CodeCompletionModelPrivate* const d;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(CodeCompletionModel::CompletionProperties)
Q_DECLARE_OPERATORS_FOR_FLAGS(CodeCompletionModel::HighlightMethods)

}

#endif // KDELIBS_KTEXTEDITOR_CODECOMPLETIONMODEL_H
