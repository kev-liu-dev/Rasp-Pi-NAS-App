#ifndef SEARCHBAR_H
#define SEARCHBAR_H

#include <QLineEdit>

/**
 * @author Harshi Kamboj
 * @class SearchBar
 * @brief A custom-styled search input field used in the top toolbar.
 *
 * This class subclasses QLineEdit and adds custom placeholder text, styling, and a clear button.
 */
class SearchBar : public QLineEdit
{
    Q_OBJECT
public:
    /**
     * @brief Constructs a styled SearchBar widget.
     * @param parent Optional parent widget.
     */
    explicit SearchBar(QWidget *parent = nullptr);
};

#endif // SEARCHBAR_H
