#include "Toolbar.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QMenu>
#include <QAction>
#include <QLabel>

/**
 * @author Harshi Kamboj
 * @brief Constructs the Toolbar widget and initializes its buttons.
 */
Toolbar::Toolbar(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(8);

    QString btnStyle = R"(
        QPushButton {
            background-color: #FFFFFF;
            color: #000000;
            border: 1px solid #CCCCCC;
            border-radius: 4px;
            padding: 4px 8px;
        }
        QPushButton:disabled {
            background-color: #F0F0F0;
            color: #AAAAAA;
        }
        QPushButton:hover:!disabled {
            background-color: #E6E6E6;
        }
    )";

    // Select All Button
    selectAllBtn = new QPushButton("Select All", this);
    selectAllBtn->setStyleSheet(btnStyle);
    connect(selectAllBtn, &QPushButton::clicked, this, &Toolbar::onSelectAllClicked);
    layout->addWidget(selectAllBtn);

    // Selected Count Label
    selectedCountLabel = new QLabel("0 selected", this);
    selectedCountLabel->setStyleSheet("font-size: 14px; color: #000000;");
    layout->addWidget(selectedCountLabel);

    layout->addStretch();

    // Rename Button
    renameBtn = new QPushButton("Rename", this);
    renameBtn->setStyleSheet(btnStyle);
    connect(renameBtn, &QPushButton::clicked, this, [this]() {
        emit renameRequested();
    });
    layout->addWidget(renameBtn);

    // Delete Button
    deleteBtn = new QPushButton("Delete", this);
    deleteBtn->setStyleSheet(btnStyle);
    connect(deleteBtn, &QPushButton::clicked, this, [this]() {
        emit deleteRequested();
    });
    layout->addWidget(deleteBtn);

    // Sort Button with Dropdown Menu
    sortBtn = new QPushButton("Sort", this);
    sortBtn->setStyleSheet(btnStyle);
    QMenu *sortMenu = new QMenu(sortBtn);
    sortMenu->setStyleSheet(
        "QMenu { background-color: #FFFFFF; color: #000000; border: 1px solid #CCCCCC; }"
        "QMenu::item:selected { background-color: #E6E6E6; color: #000000; }"
        );

    QAction *sortNameAsc  = new QAction("Name (Ascending)", sortMenu);
    QAction *sortNameDesc = new QAction("Name (Descending)", sortMenu);
    QAction *sortDateAsc  = new QAction("Date modified (Ascending)", sortMenu);
    QAction *sortDateDesc = new QAction("Date modified (Descending)", sortMenu);
    QAction *sortTypeAsc  = new QAction("Type (Ascending)", sortMenu);
    QAction *sortTypeDesc = new QAction("Type (Descending)", sortMenu);

    connect(sortNameAsc,  &QAction::triggered, this, [this]() { emit sortRequested(0); });
    connect(sortNameDesc, &QAction::triggered, this, [this]() { emit sortRequested(1); });
    connect(sortDateAsc,  &QAction::triggered, this, [this]() { emit sortRequested(2); });
    connect(sortDateDesc, &QAction::triggered, this, [this]() { emit sortRequested(3); });
    connect(sortTypeAsc,  &QAction::triggered, this, [this]() { emit sortRequested(4); });
    connect(sortTypeDesc, &QAction::triggered, this, [this]() { emit sortRequested(5); });

    sortMenu->addAction(sortNameAsc);
    sortMenu->addAction(sortNameDesc);
    sortMenu->addAction(sortDateAsc);
    sortMenu->addAction(sortDateDesc);
    sortMenu->addAction(sortTypeAsc);
    sortMenu->addAction(sortTypeDesc);

    connect(sortBtn, &QPushButton::clicked, this, [this, sortMenu]() {
        QPoint pos = this->sortBtn->mapToGlobal(QPoint(0, this->sortBtn->height()));
        sortMenu->exec(pos);
    });
    layout->addWidget(sortBtn);

    // Upload Button
    uploadBtn = new QPushButton("Upload", this);
    uploadBtn->setStyleSheet(
        "QPushButton { background-color: #14bcfb; color: #000000; padding: 8px 16px; border-radius: 4px; }"
        "QPushButton:hover:!disabled { background-color: #0056b3; }"
        "QPushButton:disabled { background-color: #F0F0F0; color: #AAAAAA; }"
        );
    connect(uploadBtn, &QPushButton::clicked, this, [this]() {
        emit uploadRequested();
    });
    layout->addWidget(uploadBtn);

    // Download Button
    downloadBtn = new QPushButton("Download", this);
    downloadBtn->setStyleSheet(
        "QPushButton { background-color: #14bcfb; color: #000000; padding: 8px 16px; border-radius: 4px; }"
        "QPushButton:hover:!disabled { background-color: #0056b3; }"
        "QPushButton:disabled { background-color: #F0F0F0; color: #AAAAAA; }"
        );
    connect(downloadBtn, &QPushButton::clicked, this, [this]() {
        emit downloadRequested();
    });
    layout->addWidget(downloadBtn);

    setLayout(layout);
    updateButtonStates();
}

/**
 * @brief Slot called when the "Select All" button is clicked.
 */
void Toolbar::onSelectAllClicked()
{
    emit selectAllToggled();
}

/**
 * @brief Updates internal selection state and the UI label based on the number of selected files.
 * @param selectedCount The number of selected files.
 * @param allSelected True if all files are selected.
 * @param noneSelected True if no files are selected.
 */
void Toolbar::onSelectionInfoChanged(int selectedCount, bool allSelected, bool noneSelected)
{
    currentSelectedCount = selectedCount;
    isAllSelected = allSelected;
    isNoneSelected = noneSelected;

    selectAllBtn->setText(isAllSelected ? "Deselect All" : "Select All");
    selectedCountLabel->setText(QString("%1 selected").arg(currentSelectedCount));

    updateButtonStates();
}

/**
 * @brief Updates the enabled/disabled state of buttons based on the current selection count.
 */
void Toolbar::updateButtonStates()
{
    renameBtn->setEnabled(currentSelectedCount == 1);
    deleteBtn->setEnabled(currentSelectedCount > 0);
    downloadBtn->setEnabled(currentSelectedCount == 1);
}
