#ifndef DISASSEMBLERDOCUMENT_H
#define DISASSEMBLERDOCUMENT_H

#include <QTextDocument>
#include <QTextCursor>
#include <QJsonObject>
#include <QColor>
#include <QUrl>
#include "../../redasm/disassembler/disassembler.h"

#define S_TO_QS(s)           QString::fromStdString(s)
#define HEX_ADDRESS(address) S_TO_QS(REDasm::hex(address, this->_disassembler->format()->bits(), false))

class DisassemblerDocument: public QObject
{
    Q_OBJECT

    private:
        typedef std::set<address_t> GeneratedBlocks;

    public:
        enum { UnknownBlock = 0,
               Address,
               IsEmptyBlock, IsFunctionBlock, IsInstructionBlock, IsLabelBlock };

        enum { NoAction = 0, GotoAction, XRefAction };

    public:
        DisassemblerDocument(REDasm::Disassembler* disassembler, const QString& theme, QTextDocument *document, const QTextCursor &cursor, QObject* parent = 0);
        QColor highlightColor() const;
        QColor seekColor() const;
        void setTheme(const QString& theme);
        void generate(address_t address, const QTextCursor &cursor);
        void update(address_t address);

    private:
        void updateInstructions(const REDasm::SymbolPtr &symbol);
        void updateFunction(const REDasm::SymbolPtr &symbol);
        void updateLabels(const REDasm::SymbolPtr &symbol);

    private:
        void appendFunctionEnd(const REDasm::InstructionPtr &lastinstruction);
        void appendLabel(const REDasm::SymbolPtr& symbol, bool replace = false);
        void appendFunctionStart(const REDasm::SymbolPtr& symbol, bool replace = false);
        void appendInstruction(const REDasm::InstructionPtr& instruction, bool replace = false);
        void appendAddress(const REDasm::InstructionPtr& instruction);
        void appendMnemonic(const REDasm::InstructionPtr& instruction);
        void appendComment(const REDasm::InstructionPtr& instruction);
        void appendOperand(const REDasm::Operand& operand, const QString& opstr);

    private:
        int getIndent(address_t address);
        const REDasm::Segment *getSegment(address_t address);
        void setMetaData(QTextCharFormat &charformat, const REDasm::SymbolPtr &symbol, bool showxrefs = false);
        void moveToBlock(address_t address);
        bool selectBlock(address_t address);
        bool isInstructionGenerated(address_t address);
        bool isLabelGenerated(address_t address);

    public:
        static QJsonObject decode(const QString &data);

    private:
        static QString encode(const QJsonObject &json);

    private:
        REDasm::Segment* _segment;
        REDasm::Disassembler* _disassembler;
        REDasm::SymbolTable* _symbols;
        GeneratedBlocks _generatedinstructions;
        QTextDocument* _document;
        QTextCursor _textcursor;
        QJsonObject _theme;
};

#endif // DISASSEMBLERDOCUMENT_H
