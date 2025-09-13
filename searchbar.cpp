#include "SearchBar.h"

/**
 * @author Harshi Kamboj
 * @brief Constructor for the SearchBar.
 *
 * Sets placeholder text, enables a clear button, and applies custom styles.
 */
SearchBar::SearchBar(QWidget *parent)
    : QLineEdit(parent)
{
    // Set placeholder text inside the search box
    setPlaceholderText("Search...");

    // Adds a small 'x' button to clear the text
    setClearButtonEnabled(true);

    // Style the search bar: padding, color, rounded edges, hover effect
    setStyleSheet(
        "QLineEdit { font-size: 14px; padding: 6px; background-color: #ECECEC; border-radius: 4px; color: #000000; }"
        "QLineEdit:focus { background-color: #FFFFFF; }"
        );
}
