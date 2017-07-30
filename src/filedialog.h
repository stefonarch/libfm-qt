#ifndef FM_FILEDIALOG_H
#define FM_FILEDIALOG_H

#include "libfmqtglobals.h"
#include "core/filepath.h"
#include "ui_filedialog.h"

#include <QFileDialog>
#include <QRegExp>
#include <vector>

namespace Fm {

class CachedFolderModel;
class ProxyFolderModel;

class LIBFM_QT_API FileDialog : public QDialog {
    Q_OBJECT
public:
    explicit FileDialog(QWidget *parent = 0, FilePath path = FilePath::homeDir());

    ~FileDialog() = default;

    // Some QFileDialog compatible interface
    QFileDialog::Options options() const {
        return options_;
    }

    void setOptions(QFileDialog::Options options) {
        options_ = options;
    }

    // interface for QPlatformFileDialogHelper
    bool defaultNameFilterDisables() const; // FIXME: what's this?

    void setDirectory(const QUrl &directory);

    QUrl directory() const;

    void selectFile(const QUrl &filename);

    QList<QUrl> selectedFiles();

    void setFilter();

    void selectNameFilter(const QString &filter);

    QString selectedNameFilter() const {
        return currentNameFilter_;
    }

    bool isSupportedUrl(const QUrl &url);

    // options

    // not yet supported
    QDir::Filters filter() const {
        return filters_;
    }
    // not yet supported
    void setFilter(QDir::Filters filters);

    void setViewMode(QFileDialog::ViewMode mode);
    QFileDialog::ViewMode viewMode() const {
        return viewMode_;
    }

    void setFileMode(QFileDialog::FileMode mode);
    QFileDialog::FileMode fileMode() const {
        return fileMode_;
    }

    void setAcceptMode(QFileDialog::AcceptMode mode);
    QFileDialog::AcceptMode acceptMode() const {
        return acceptMode_;
    }

    // not supported
    void setSidebarUrls(const QList<QUrl> &urls) {}
    // not supported
    QList<QUrl> sidebarUrls() const {}

    void setNameFilters(const QStringList &filters);
    QStringList nameFilters() const {
        return nameFilters_;
    }

    void setMimeTypeFilters(const QStringList &filters);
    QStringList mimeTypeFilters() const {
        return mimeTypeFilters_;
    }

    void setDefaultSuffix(const QString &suffix) {
        defaultSuffix_ = suffix;
    }
    QString defaultSuffix() const {
        return defaultSuffix_;
    }

    // not supported yet
    void setHistory(const QStringList &paths);
    // not supported yet
    QStringList history() const;

    void setLabelText(QFileDialog::DialogLabel label, const QString &text);
    QString labelText(QFileDialog::DialogLabel label) const;

    bool isLabelExplicitlySet(QFileDialog::DialogLabel label);

#if 0
    // not supported yet
    QUrl initialDirectory() const;
    // not supported yet
    void setInitialDirectory(const QUrl &directory);

    QString initiallySelectedNameFilter() const;
    void setInitiallySelectedNameFilter(const QString & filter);

    QList<QUrl> initiallySelectedFiles() const;
    void setInitiallySelectedFiles(const QList<QUrl> &fileUrls);
#endif

public Q_SLOTS:
    void setDirectoryPath(FilePath directory);

Q_SIGNALS:
    void fileSelected(const QUrl &file);
    void filesSelected(const QList<QUrl> &files);
    void currentChanged(const QUrl &path);
    void directoryEntered(const QUrl &directory);
    void filterSelected(const QString &filter);

private:

    class FileDialogFilter: public ProxyFolderModelFilter {
    public:
        FileDialogFilter(FileDialog* dlg): dlg_{dlg} {}
        virtual bool filterAcceptsRow(const ProxyFolderModel* model, const std::shared_ptr<const Fm::FileInfo>& info) const override;
        void update();

        FileDialog* dlg_;
        std::vector<QRegExp> patterns_;

    };

private:
    Ui::FileDialog ui;
    CachedFolderModel* folderModel_;
    ProxyFolderModel* proxyModel_;
    FilePath directoryPath_;

    QFileDialog::Options options_;
    QDir::Filters filters_;
    QFileDialog::ViewMode viewMode_;
    QFileDialog::FileMode fileMode_;
    QFileDialog::AcceptMode acceptMode_;
    QStringList nameFilters_;
    QStringList mimeTypeFilters_;
    QString defaultSuffix_;
    unsigned int isLabelExplicitlySetMask_;
    FileDialogFilter modelFilter_;
    QString currentNameFilter_;
};


} // namespace Fm
#endif // FM_FILEDIALOG_H