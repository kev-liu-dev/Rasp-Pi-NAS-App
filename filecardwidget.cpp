#include "FileCardWidget.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QPixmap>

/**
 * @author Harshi Kamboj
 * @brief Constructs the visual representation of a file card.
 *
 * Initializes layout, styles, and button interactions for favorite and select functionality.
 */
FileCardWidget::FileCardWidget(FileData &data, int index, QWidget *parent)
    : QFrame(parent), m_file(data), m_index(index)
{
    // Style the card
    setStyleSheet(
        "QFrame { background-color: #FFFFFF; border: 1px solid #d3d3d3; border-radius: 8px; }"
        "QFrame:hover { background-color: #15BCFF; }"
        );
    setFixedSize(150, 220);

    QVBoxLayout *cardLayout = new QVBoxLayout(this);
    cardLayout->setContentsMargins(10, 10, 10, 10);
    cardLayout->setSpacing(5);

    // File icon setup
    QLabel *iconLabel = new QLabel(this);
    QPixmap pix(QString(":/icons/%1").arg(m_file.iconName));
    pix = pix.scaled(48, 48, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    iconLabel->setPixmap(pix);
    iconLabel->setAlignment(Qt::AlignHCenter);
    cardLayout->addWidget(iconLabel);

    // Display file name
    QLabel *nameLabel = new QLabel(m_file.fileName, this);
    nameLabel->setStyleSheet("font-weight: bold; font-size: 12px; color: #000000;");
    nameLabel->setAlignment(Qt::AlignHCenter);
    cardLayout->addWidget(nameLabel);

    // Display last modified date
    QString dateStr = m_file.dateModified.toString("yyyy-MM-dd hh:mm");
    QLabel *dateLabel = new QLabel(QString("Modified: %1").arg(dateStr), this);
    dateLabel->setStyleSheet("color: #555555; font-size: 10px;");
    dateLabel->setAlignment(Qt::AlignHCenter);
    cardLayout->addWidget(dateLabel);

    // Favorite button setup
    favoriteBtn = new QPushButton(this);
    favoriteBtn->setStyleSheet(
        "QPushButton { background-color: #FFFFFF; border: 1px solid #CCCCCC; border-radius: 4px; padding: 2px 4px; color: #000000; }"
        "QPushButton:hover { background-color: #F0F0F0; }"
        );
    connect(favoriteBtn, &QPushButton::clicked, this, &FileCardWidget::onFavoriteClicked);
    cardLayout->addWidget(favoriteBtn);
    cardLayout->addSpacing(12);// Adds spacing in between the two buttons

    // Select button setup
    selectBtn = new QPushButton(this);
    selectBtn->setStyleSheet(
        "QPushButton { background-color: #FFFFFF; border: 1px solid #CCCCCC; border-radius: 4px; padding: 2px 4px; color: #000000; }"
        "QPushButton:hover { background-color: #F0F0F0; }"
        );
    connect(selectBtn, &QPushButton::clicked, this, &FileCardWidget::onSelectClicked);
    cardLayout->addWidget(selectBtn);

    cardLayout->addStretch();

    // Set initial button states
    updateButtons();
}

/**
 * @brief Slot: Toggles the favorite state of the file and emits a signal.
 */
void FileCardWidget::onFavoriteClicked()
{
    m_file.isFavorite = !m_file.isFavorite;
    updateButtons();
    emit favoriteToggled(m_index, m_file.isFavorite);
}

/**
 * @brief Slot: Toggles the selected state of the file and emits a signal.
 */
void FileCardWidget::onSelectClicked()
{
    m_file.isSelected = !m_file.isSelected;
    updateButtons();
    emit selectedToggled(m_index, m_file.isSelected);
}

/**
 * @brief Updates the button text for favorite and select buttons based on current file state.
 */
void FileCardWidget::updateButtons()
{
    favoriteBtn->setText(m_file.isFavorite ? "Unfavorite" : "Favorite");
    selectBtn->setText(m_file.isSelected ? "Deselect" : "Select");
}
