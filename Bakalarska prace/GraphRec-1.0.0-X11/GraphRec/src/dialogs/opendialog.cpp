/* GraphRec
 * Copyright (c) 2009 Petr Koupy <petr.koupy@gmail.com>
 *
 * GNU General Public License Usage
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QtCore/QSettings>
#include <QtCore/QSignalMapper>
#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QEvent>
#include <QtCore/QFile>
#include <QtCore/QIODevice>
#include <QtCore/QPoint>
#include <QtCore/QPointF>
#include <QtGui/QWidget>
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QMenu>
#include <QtGui/QAction>
#include <QtGui/QTreeWidgetItem>

#include "dialogs/opendialog.h"
#include "ui_opendialog.h"
#include "parsers/parser.h"
#include "auxiliary/treewidgetitem.h"
#include "factory.h"

OpenDialog::OpenDialog(QWidget* parent) :
        QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint),
        m_ui(new Ui::OpenDialog)
{
    m_ui->setupUi(this);
    m_signalMapperValidators = new QSignalMapper(this);

    headerTemplate.append(Parser::hID);
    headerTemplate.append(Parser::hLine);
    headerTemplate.append(Parser::hNodes);
    headerTemplate.append(Parser::hEdges);
    headerTemplate.append(Parser::hEntities);
    headerTemplate.append(Parser::hTimeSteps);
    headerTemplate.append(Parser::hMovements);
    headerTemplate.append(Parser::hFilePosition);
    headerTemplate.append(Parser::hParser);
    headerTemplate.append(Parser::hValidator);
    headerTemplate.append(Parser::hLayouter);

    m_ui->listFound->setColumnCount(headerTemplate.size());
    m_ui->listChosen->setColumnCount(headerTemplate.size());
    QStringList headerLabels;
    foreach (Parser::HeaderItem item, headerTemplate)
    {
        switch (item)
        {
            case Parser::hID:
                headerLabels.append(tr("Name"));
                break;
            case Parser::hNodes:
                headerLabels.append(tr("Nodes"));
                break;
            case Parser::hEdges:
                headerLabels.append(tr("Edges"));
                break;
            case Parser::hEntities:
                headerLabels.append(tr("Entities"));
                break;
            case Parser::hTimeSteps:
                headerLabels.append(tr("Timesteps"));
                break;
            case Parser::hMovements:
                headerLabels.append(tr("Movements"));
                break;
            case Parser::hLine:
                headerLabels.append(tr("Line"));
                break;
            case Parser::hFilePosition:
                headerLabels.append(tr("Position"));
                break;
            case Parser::hParser:
                headerLabels.append(tr("Parser"));
                break;
            case Parser::hValidator:
                headerLabels.append(tr("Validator"));
                break;
            case Parser::hLayouter:
                headerLabels.append(tr("Layouter"));
                break;
            default:
                headerLabels.append("");
                break;
        }
    }
    HideColumn(Parser::hFilePosition);
    HideColumn(Parser::hParser);
    HideColumn(Parser::hLayouter);

    m_ui->listFound->setContextMenuPolicy(Qt::CustomContextMenu);
    m_ui->listChosen->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_ui->listFound, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(ShowContextMenu(const QPoint&)));
    connect(m_ui->listChosen, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(ShowContextMenu(const QPoint&)));
    m_ui->listFound->setHeaderLabels(headerLabels);
    m_ui->listChosen->setHeaderLabels(headerLabels);
    for (int i = 0; i < headerTemplate.size(); i++)
    {
        m_ui->listFound->resizeColumnToContents(i);
        m_ui->listChosen->resizeColumnToContents(i);
    }
    m_ui->listFound->sortByColumn(0, Qt::AscendingOrder);
    m_ui->listChosen->sortByColumn(0, Qt::AscendingOrder);
    connect(m_ui->listFound, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(ItemDoubleClickedFound(QTreeWidgetItem*, int)));
    connect(m_ui->listChosen, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(ItemDoubleClickedChosen(QTreeWidgetItem*, int)));

    m_contextMenu = new QMenu(tr("Context menu"), this);
    QMenu* subMenuValidators = new QMenu(tr("Set &Validator"), m_contextMenu);
    m_contextMenu->addMenu(subMenuValidators);
    foreach (QString validator, Factory::GetServantNames(Factory::sValidator))
    {
        QAction* action = new QAction(validator, subMenuValidators);
        connect(action, SIGNAL(triggered()), m_signalMapperValidators, SLOT(map()));
        m_signalMapperValidators->setMapping(action, validator);
        subMenuValidators->addAction(action);
    }
    connect(m_signalMapperValidators, SIGNAL(mapped(const QString&)), this, SLOT(SetValidator(const QString&)));
}

OpenDialog::~OpenDialog()
{
    delete m_contextMenu;
    delete m_signalMapperValidators;
    delete m_ui;
}

void OpenDialog::on_buttonAddFiles_clicked()
{
    QSettings settings;
    QStringList parsers = Factory::GetServantNames(Factory::sParser);
    parsers.append(tr("All Files (*)"));
    /**/
    QFileDialog openFileDialog(this, tr("Add files"), "", parsers.join(";;"));
    openFileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    openFileDialog.setFileMode(QFileDialog::ExistingFiles);
    openFileDialog.setViewMode(QFileDialog::Detail);
    QString defaultFilter = settings.value("OpenDialog/Parser", openFileDialog.selectedFilter()).toString();
    openFileDialog.selectFilter(defaultFilter);
    /**/
    QStringList fileNames;
    QString parserName;

    //if (true)
    if (openFileDialog.exec())
    {
        /**/
        fileNames = openFileDialog.selectedFiles();
        parserName = openFileDialog.selectedNameFilter();
        /**/

        /*
        fileNames.append(".\\test1.txt");
        fileNames.append(".\\test2.txt");
        parserName = "Multirobot (*.txt)";
        */

        /*
        fileNames.append(".\\test2.xml");
        parserName = "GraphRec (*.xml)";
        */

        if (!fileNames.isEmpty())
        {
            foreach (QString fileName, fileNames)
            {
                QFile file;
                file.setFileName(fileName);
                if (!file.open(QIODevice::ReadOnly))
                {
                    QMessageBox::warning(this, tr("Warning"), file.errorString());
                    return;
                }
                Parser* parser = qobject_cast<Parser*>(Factory::CreateServant(Factory::sParser, parserName));
                if (parser)
                {
                    QTreeWidgetItem* item = parser->ParseFile(file, headerTemplate);
                    if (item)
                    {
                        FillMissingInfo(item);
                        m_ui->listFound->addTopLevelItem(item);
                        m_ui->listFound->expandItem(item);
                    }
                }
                file.close();
            }
        }
    }
    for (int i = 0; i < headerTemplate.size(); i++)
        m_ui->listFound->resizeColumnToContents(i);
    /**/
    settings.setValue("OpenDialog/Parser", openFileDialog.selectedFilter());
    /**/
}

void OpenDialog::on_buttonToChosen_clicked()
{
    Move(m_ui->listFound, m_ui->listChosen);
}

void OpenDialog::on_buttonToFound_clicked()
{
    Move(m_ui->listChosen, m_ui->listFound);
}

void OpenDialog::on_buttonOpen_clicked()
{
    for (int i = 0; i < m_ui->listChosen->topLevelItemCount(); i++)
    {
        QTreeWidgetItem* parent = m_ui->listChosen->topLevelItem(i);
        if (parent)
        {
            for (int j = 0; j < parent->childCount(); j++)
            {
                QTreeWidgetItem* child = parent->child(j);
                if (child)
                {
                    SolutionInfo info;
                    info.fileName = parent->toolTip(0);
                    bool ok = false;
                    info.filePosition = child->text(headerTemplate.indexOf(Parser::hFilePosition)).toLongLong(&ok);
                    if (!ok)
                        continue;
                    info.layouter = child->text(headerTemplate.indexOf(Parser::hLayouter));
                    info.parser = child->text(headerTemplate.indexOf(Parser::hParser));
                    info.validator = child->text(headerTemplate.indexOf(Parser::hValidator));
                    acceptedSolutions.append(info);
                }
            }
        }
    }
    this->accept();
}

void OpenDialog::on_buttonCancel_clicked()
{
    this->reject();
}

void OpenDialog::changeEvent(QEvent* e)
{
    QDialog::changeEvent(e);
    switch (e->type())
    {
        case QEvent::LanguageChange:
            m_ui->retranslateUi(this);
            break;
        default:
            break;
    }
}

void OpenDialog::SetValidator(const QString& validator)
{
    if (!m_senderTreeWidget)
        return;
    int ixValidator = headerTemplate.indexOf(Parser::hValidator);
    if (ixValidator < 0)
        return;
    foreach (QTreeWidgetItem* item, m_senderTreeWidget->selectedItems())
    {
        if (m_senderTreeWidget->indexOfTopLevelItem(item) < 0)
            item->setText(ixValidator, validator);
        else
        {
            for (int i = 0; i < item->childCount(); i++)
            {
                QTreeWidgetItem* child = item->child(i);
                if (child)
                    child->setText(ixValidator, validator);
            }
        }
    }
}

void OpenDialog::ShowContextMenu(const QPoint& position)
{
    m_senderTreeWidget = qobject_cast<QTreeWidget*>(sender());
    if (!m_senderTreeWidget)
        return;

    QTreeWidgetItem* item = m_senderTreeWidget->itemAt(position);
    if (!item)
        return;
    if (!item->isSelected())
        item->setSelected(true);

    m_contextMenu->exec(m_senderTreeWidget->viewport()->mapToGlobal(position));
}

void OpenDialog::ItemDoubleClickedFound(QTreeWidgetItem* item, int column)
{
    Q_UNUSED(item)
    Q_UNUSED(column)
    on_buttonToChosen_clicked();
}

void OpenDialog::ItemDoubleClickedChosen(QTreeWidgetItem* item, int column)
{
    Q_UNUSED(item)
    Q_UNUSED(column)
    on_buttonToFound_clicked();
}

void OpenDialog::HideColumn(Parser::HeaderItem what)
{
    int index = headerTemplate.indexOf(what);
    if (index >= 0)
    {
        m_ui->listFound->hideColumn(index);
        m_ui->listChosen->hideColumn(index);
    }
}

void OpenDialog::FillMissingInfo(QTreeWidgetItem* item)
{
    QSettings settings;
    int ixValidator = headerTemplate.indexOf(Parser::hValidator);
    QString validator = "";
    QStringList validators = Factory::GetServantNames(Factory::sValidator);
    if (!validators.isEmpty())
        validator = settings.value("GraphView/Validator", validators[0]).toString();

    int ixLayouter = headerTemplate.indexOf(Parser::hLayouter);
    QString layouter = "";
    QStringList layouters = Factory::GetServantNames(Factory::sLayouter);
    if (!layouters.isEmpty())
        layouter = settings.value("GraphView/Layouter", layouters[0]).toString();

    for (int i = 0; i < item->childCount(); i++)
    {
        QTreeWidgetItem* child = item->child(i);
        if (child)
        {
            if (ixValidator >= 0 && child->text(ixValidator).isEmpty())
                child->setText(ixValidator, validator);
            if (ixLayouter >= 0 && child->text(ixLayouter).isEmpty())
                child->setText(ixLayouter, layouter);
        }
    }
}

void OpenDialog::Move(QTreeWidget* source, QTreeWidget* destination)
{
    if (source && destination)
    {
        foreach (QTreeWidgetItem* item, source->selectedItems())
        {
            if (item && item->treeWidget() != destination)
            {
                if (source->indexOfTopLevelItem(item) < 0)
                {
                    QTreeWidgetItem* parentOld = item->parent();
                    if (parentOld && !parentOld->isSelected())
                    {
                        QTreeWidgetItem* parentNew = NULL;
                        for (int i = 0; i < destination->topLevelItemCount(); i++)
                        {
                            QTreeWidgetItem* parentCandidate = destination->topLevelItem(i);
                            if (parentCandidate && parentCandidate->toolTip(0) == parentOld->toolTip(0))
                                parentNew = parentCandidate;
                        }
                        if (!parentNew)
                        {
                            parentNew = new QTreeWidgetItem();
                            parentNew->setToolTip(0, parentOld->toolTip(0));
                            parentNew->setText(0, parentOld->text(0));
                            parentNew->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
                            destination->addTopLevelItem(parentNew);
                            if (parentOld->isExpanded())
                                destination->expandItem(parentNew);
                        }
                        parentNew->addChild(parentOld->takeChild(parentOld->indexOfChild(item)));
                        for (int i = 0; i < headerTemplate.size(); i++)
                            destination->resizeColumnToContents(i);
                        if (parentOld->childCount() == 0)
                        {
                            source->takeTopLevelItem(source->indexOfTopLevelItem(parentOld));
                            delete parentOld;
                        }
                        for (int i = 0; i < headerTemplate.size(); i++)
                            source->resizeColumnToContents(i);
                    }
                }
                else
                {
                    QTreeWidgetItem* parentNew = NULL;
                    for (int i = 0; i < destination->topLevelItemCount(); i++)
                    {
                        QTreeWidgetItem* parentCandidate = destination->topLevelItem(i);
                        if (parentCandidate && parentCandidate->toolTip(0) == item->toolTip(0))
                            parentNew = parentCandidate;
                    }
                    if (!parentNew)
                    {
                        parentNew = new QTreeWidgetItem();
                        parentNew->setToolTip(0, item->toolTip(0));
                        parentNew->setText(0, item->text(0));
                        parentNew->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
                        destination->addTopLevelItem(parentNew);
                        if (item->isExpanded())
                            destination->expandItem(parentNew);
                    }
                    parentNew->addChildren(item->takeChildren());
                    for (int i = 0; i < headerTemplate.size(); i++)
                        destination->resizeColumnToContents(i);
                    source->takeTopLevelItem(source->indexOfTopLevelItem(item));
                    delete item;
                    for (int i = 0; i < headerTemplate.size(); i++)
                        source->resizeColumnToContents(i);
                }
            }
        }
    }
}
