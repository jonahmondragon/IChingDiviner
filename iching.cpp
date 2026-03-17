#include "iching.h"
#include <QFile>
#include <QTextStream>
#include <QRandomGenerator>
#include <QRegularExpression>
#include <QDebug>
#include<QJsonObject>
#include<QJsonDocument>
#include<QCoreApplication>
#include<QThread>
#include<QDateTime>

IChing::IChing(QObject *parent)
    : QObject(parent)
{
    rawData =
        "01䷀ The Creative (Heaven) 乾 Qián 111111\n"
        "02䷁ The Receptive (Earth) 坤 Kūn 000000\n"
        "03䷂ Difficulty at the Beginning (Water over Thunder) 屯 Zhūn 010001\n"
        "04䷃ Youthful Folly (Mountain over Water) 蒙 Méng 100010\n"
        "05䷄ Waiting (Lake over Water) 需 Xū 010111\n"
        "06䷅ Conflict (Heaven over Water) 訟 Sòng 111010\n"
        "07䷆ The Army (Earth over Lake) 師 Shī 000010\n"
        "08䷇ Holding Together (Lake over Earth) 比 Bǐ 010000\n"
        "09䷈ The Taming Power of the Small (Wind over Heaven) 小畜 Xiǎo Chù 110111\n"
        "10䷉ Treading (The Taming Power of the Small) 履 Lǚ 111011\n"
        "11䷊ Peace (Heaven over Earth) 泰 Tài 000111\n"
        "12䷋ Standstill (Earth over Heaven) 否 Pǐ 111000\n"
        "13䷌ Fellowship with Men (Heaven over Fire) 同人 Tóng Rén 111101\n"
        "14䷍ Possession in Great Measure (Fire over Heaven) 大有 Dà Yǒu 101111\n"
        "15䷎ Modesty (Earth over Mountain) 謙 Qiān 000100\n"
        "16䷏ Enthusiasm (Thunder over Lake) 豫 Yù 001000\n"
        "17䷐ Following (Lake over Thunder) 隨 Suí 011001\n"
        "18䷑ Work on what has been spoiled (Mountain over Wind) 蠱 Gǔ 100110\n"
        "19䷒ Approach (Earth over Lake) 臨 Lín 000011\n"
        "20䷓ Contemplation (Heaven over Earth) 觀 Guān 110000\n"
        "21䷔ Biting Through (Thunder over Lake) 噬嗑 Shì Kè 101001\n"
        "22䷕ Grace (Lake over Fire) 賁 Bì 100101\n"
        "23䷖ Splitting Apart (Mountain over Earth) 剝 Bō 100000\n"
        "24䷗ Return (Earth over Mountain) 復 Fù 000001\n"
        "25䷘ Innocence (Heaven over Wind) 無妄 Wú Wàng 111001\n"
        "26䷙ Great Taming (Mountain over Heaven) 大畜 Dà Chù 100111\n"
        "27䷚ Nourishment (Earth over Wind) 頤 Yí 100001\n"
        "28䷛ Greatness in Excess (Lake over Thunder) 大過 Dà Guò 011110\n"
        "29䷜ The Abysmal (Water over Water) 坎 Kǎn 010010\n"
        "30䷝ The Clinging, Fire (Fire over Fire) 離 Lí 101101\n"
        "31䷞ Influence (Lake over Mountain) 咸 Xián 011100\n"
        "32䷟ Duration (Wind over Thunder) 恆 Héng 001110\n"
        "33䷠ Retreat (Heaven over Mountain) 遯 Dùn 111100\n"
        "34䷡ The Power of the Great (Thunder over Heaven) 大壯 Dà Zhuàng 001111\n"
        "35䷢ Progress (Fire over Lake) 晉 Jìn 101000\n"
        "36䷣ Darkening of the Light (Mountain over Lake) 明夷 Míng Yí 000101\n"
        "37䷤ The Family (Wind over Earth) 家人 Jiā Rén 110101\n"
        "38䷥ Opposition (Lake over Thunder) 睽 Kuí 101011\n"
        "39䷦ Obstruction (Water over Mountain) 蹇 Jiǎn 010100\n"
        "40䷧ Deliverance (Thunder over Water) 解 Xiè 001010\n"
        "41䷨ Decrease (Lake over Earth) 損 Sǔn 100011\n"
        "42䷩ Increase (Wind over Lake) 益 Yì 110001\n"
        "43䷪ Breakthrough (Heaven over Lake) 夬 Guài 011111\n"
        "44䷫ Coming to Meet (Wind over Heaven) 姤 Gòu 111110\n"
        "45䷬ Gathering Together (Earth over Lake) 萃 Cuì 011000\n"
        "46䷭ Pushing Upward (Lake over Mountain) 升 Shēng 000110\n"
        "47䷮ Oppression (Water over Lake) 困 Kùn 011010\n"
        "48䷯ The Well (Wind over Water) 井 Jǐng 010110\n"
        "49䷰ Revolution (Lake over Fire) 革 Gé 011101\n"
        "50䷱ The Cauldron (Fire over Thunder) 鼎 Dǐng 101110\n"
        "51䷲ The Arousing (Thunder over Thunder) 震 Zhèn 001001\n"
        "52䷳ Keeping Still (Mountain over Mountain) 艮 Gèn 100100\n"
        "53䷴ Development (Wind over Earth) 漸 Jiàn 110100\n"
        "54䷵ The Marrying Maiden (Lake over Thunder) 歸妹 Guī Mèi 001011\n"
        "55䷶ Abundance (Fire over Earth) 豐 Fēng 001101\n"
        "56䷷ The Traveler (Mountain over Fire) 旅 Lǚ 101100\n"
        "57䷸ The Gentle (Wind over Lake) 巽 Xùn 110110\n"
        "58䷹ Joy (Lake over Lake) 兌 Duì 011011\n"
        "59䷺ Dispersion (Wind over Water) 渙 Huàn 110010\n"
        "60䷻ Limitation (Lake over Wind) 節 Jié 010011\n"
        "61䷼ Inner Truth (Wind over Mountain) 中孚 Zhōng Fú 110011\n"
        "62䷽ Preponderance of the Small (Lake over Fire) 小過 Xiǎo Guò 001100\n"
        "63䷾ After Completion (Water over Thunder) 既濟 Jì Jì 010101\n"
        "64䷿ Before Completion (Thunder over Water) 未濟 Wèi Jì 101010";

    parseHexagrams(rawData);
    generateSeed();
}

void IChing::reset()
{
    currentLines.clear();
}



QList<int> IChing::performSingleToss()
{
    QList<int> result;
    for (int i = 0; i < 3; i++) {
        //result.append(QRandomGenerator::global()->bounded(2)); // 0 or 1
        result.append(randomGen.bounded(2)); // Use randomGen instead of QRandomGenerator::global()
    }
    return result;
}

QList<QList<int>> IChing::performFullDivination()
{
    currentLines.clear();
    QList<QList<int>> allCoinTosses;


    for (int i = 0; i < 6; i++) {

        // Use the existing performSingleToss method
        QList<int> coinToss = performSingleToss();
        allCoinTosses.append(coinToss);

        int sum = coinToss[0] + coinToss[1] + coinToss[2];
        QString lineType;
        switch (sum) {
        case 0: lineType = "2"; break; // changing yin
        case 1: lineType = "0"; break; // stable yin
        case 2: lineType = "1"; break; // stable yang
        case 3: lineType = "3"; break; // changing yang
        }

        //currentLines.prepend(lineType); // reverse order for bottom-to-top
        currentLines.append(lineType);  // First toss at index 0 (bottom)
    }
    return allCoinTosses;
}


QString IChing::getResultingBinary()
{
    QString sourceBinary;
    for (const QString &line : currentLines) {
        if (line == "0" || line == "2") sourceBinary += "0"; // yin lines
        else if (line == "1" || line == "3") sourceBinary += "1"; // yang lines
    }
    return sourceBinary;
}

QStringList IChing::getCurrentLines()
{
    return currentLines;
}

QStringList IChing::getModifiedLines()
{
    QStringList modified;
    for (const QString &line : currentLines) {
        if (line == "2") modified.append("1"); // changing yin -> yang
        else if (line == "3") modified.append("0"); // changing yang -> yin
        else modified.append(line); // stable lines remain the same
    }
    return modified;
}


HexagramInfo IChing::getSourceHexagram()
{
    if (currentLines.size() < 6) {
        return HexagramInfo(); // Return empty if not complete
    }

    QString binary;
    // Reverse the order for binary representation
    for (int i = currentLines.size() - 1; i >= 0; i--) {
        const QString &line = currentLines[i];
        if (line == "0" || line == "2") {
            binary += "0"; // yin lines (broken)
        } else if (line == "1" || line == "3") {
            binary += "1"; // yang lines (solid)
        }
    }

    return getHexagramFromBinary(binary);
}

HexagramInfo IChing::getModifiedHexagram()
{
    if (currentLines.size() < 6) {
        return HexagramInfo(); // Return empty if not complete
    }

    // Check if there are any changing lines
    bool hasChangingLines = false;
    for (const QString &line : currentLines) {
        if (line == "2" || line == "3") {
            hasChangingLines = true;
            break;
        }
    }

    if (!hasChangingLines) {
        return HexagramInfo(); // Return empty if no changing lines
    }

    QString binary;
    // Reverse the order for binary representation
    for (int i = currentLines.size() - 1; i >= 0; i--) {
        const QString &line = currentLines[i];
        if (line == "0") binary += "0"; // stable yin
        else if (line == "1") binary += "1"; // stable yang
        else if (line == "2") binary += "1"; // changing yin -> yang
        else if (line == "3") binary += "0"; // changing yang -> yin
    }

    return getHexagramFromBinary(binary);
}


void IChing::loadHexagramData(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Cannot open file:" << filePath;
        return;
    }

    QTextStream in(&file);
    QString rawData = in.readAll();
    file.close();

    parseHexagrams(rawData);
}


void IChing::parseHexagrams(const QString &rawData)
{
    hexagrams.clear();
    // Split the raw data by lines
    QStringList lines = rawData.split('\n', Qt::SkipEmptyParts);

    for (const QString &line : lines) {
        // Each line format: "01䷀ The Creative (Heaven) 乾 Qián 111111"
        HexagramInfo hex;

        // Extract number (digits at the beginning)
        QRegularExpression numRegex("^(\\d+)");
        QRegularExpressionMatch numMatch = numRegex.match(line);
        if (numMatch.hasMatch()) {
            hex.number = numMatch.captured(1);
        }

        // Extract symbol (Unicode character after the number)
        QRegularExpression symbolRegex("^\\d+(\\S)");
        QRegularExpressionMatch symbolMatch = symbolRegex.match(line);
        if (symbolMatch.hasMatch()) {
            hex.symbol = symbolMatch.captured(1);
            if (!hex.symbol.isEmpty()) {
                hexNumberToUnicode[hex.number.toInt()] = hex.symbol.at(0).unicode();
            }
        }

        // Extract name (text between symbol and opening parenthesis)
        int nameStart = line.indexOf(hex.symbol) + hex.symbol.length();
        int nameEnd = line.indexOf('(', nameStart);
        if (nameEnd > nameStart) {
            hex.name = line.mid(nameStart, nameEnd - nameStart).trimmed();
        }

        // Extract description (text inside parentheses)
        int descStart = nameEnd + 1;
        int descEnd = line.indexOf(')', descStart);
        if (descEnd > descStart) {
            hex.description = line.mid(descStart, descEnd - descStart).trimmed();
        }

        // Extract Chinese name and pinyin (after the closing parenthesis)

        if (descEnd > 0) {
            QStringList remainingParts = line.mid(descEnd + 1).trimmed().split(' ', Qt::SkipEmptyParts);
            if (remainingParts.size() >= 2) {
                hex.chineseName = remainingParts[0];
                hex.pinyin = remainingParts[1];
            }
        }



        // Extract binary (last part of the line)
        QRegularExpression binaryRegex("(\\d{6})$");
        QRegularExpressionMatch binaryMatch = binaryRegex.match(line);
        if (binaryMatch.hasMatch()) {
            hex.binary = binaryMatch.captured(1);
        }

        hexagrams.append(hex);


    }

}



HexagramInfo IChing::getHexagramFromBinary(const QString &binary)
{
    for (const HexagramInfo &hex : hexagrams) {
        if (hex.binary == binary)
            return hex;
    }
    return {}; // return empty hex if not found
}

/////////////////////

void IChing::parseHexagramsFromJson(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }

    QByteArray data = file.readAll();
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        return;
    }

    QJsonObject root = doc.object();

    for (const QString &key : root.keys()) {
        QJsonObject hexObj = root[key].toObject();

        // Find existing hexagram by number or create new one
        int hexNum = key.toInt();
        bool found = false;

        for (int i = 0; i < hexagrams.size(); i++) {
            if (hexagrams[i].number.toInt() == hexNum) {
                // Update existing hexagram with Wilhelm data
                if (hexObj.contains("wilhelm_symbolic")) {
                    hexagrams[i].description = hexObj["wilhelm_symbolic"].toString();
                }

                // Parse judgment
                if (hexObj.contains("wilhelm_judgment") && hexObj["wilhelm_judgment"].isObject()) {
                    QJsonObject judgmentObj = hexObj["wilhelm_judgment"].toObject();
                    hexagrams[i].judgment.text = judgmentObj["text"].toString();
                    hexagrams[i].judgment.comments = judgmentObj["comments"].toString();
                }

                // Parse image
                if (hexObj.contains("wilhelm_image") && hexObj["wilhelm_image"].isObject()) {
                    QJsonObject imageObj = hexObj["wilhelm_image"].toObject();
                    hexagrams[i].image.text = imageObj["text"].toString();
                    hexagrams[i].image.comments = imageObj["comments"].toString();
                }

                // Parse lines
                if (hexObj.contains("wilhelm_lines") && hexObj["wilhelm_lines"].isObject()) {
                    QJsonObject linesObj = hexObj["wilhelm_lines"].toObject();
                    for (const QString &lineKey : linesObj.keys()) {
                        int lineNum = lineKey.toInt();
                        QJsonObject lineObj = linesObj[lineKey].toObject();
                        WilhelmLine line;
                        line.text = lineObj["text"].toString();
                        line.comments = lineObj["comments"].toString();
                        hexagrams[i].lines[lineNum] = line;
                    }
                }

                // Parse above trigram
                if (hexObj.contains("wilhelm_above") && hexObj["wilhelm_above"].isObject()) {
                    QJsonObject aboveObj = hexObj["wilhelm_above"].toObject();
                    hexagrams[i].above.chinese = aboveObj["chinese"].toString();
                    hexagrams[i].above.symbolic = aboveObj["symbolic"].toString();
                    hexagrams[i].above.alchemical = aboveObj["alchemical"].toString();
                }

                // Parse below trigram
                if (hexObj.contains("wilhelm_below") && hexObj["wilhelm_below"].isObject()) {
                    QJsonObject belowObj = hexObj["wilhelm_below"].toObject();
                    hexagrams[i].below.chinese = belowObj["chinese"].toString();
                    hexagrams[i].below.symbolic = belowObj["symbolic"].toString();
                    hexagrams[i].below.alchemical = belowObj["alchemical"].toString();
                }

                found = true;
                break;
            }
        }

        if (!found) {
        }
    }

}

QList<int> IChing::getChangingLinePositions()
{
    QList<int> changingLines;

    for (int i = 0; i < currentLines.size(); ++i) {
        // currentLines[i] is line i+1 (bottom to top)
        if (currentLines[i] == "2" || currentLines[i] == "3") {
            changingLines.append(i + 1); // Convert to 1-based line number
        }
    }

    return changingLines;
}


void IChing::generateSeed() {


    // Start with current time in milliseconds for basic randomness
    qint64 seed = QDateTime::currentMSecsSinceEpoch();

    // Add process ID and thread ID for additional uniqueness
    seed ^= (qint64)QCoreApplication::applicationPid();
    seed ^= (qint64)QThread::currentThreadId();

    // Add a truly random value from Qt's secure random generator
    QRandomGenerator secureRandom = QRandomGenerator::securelySeeded();
    seed ^= secureRandom.generate64();

    // Add mouse entropy data if available (from shuffling)
    for (const qint64& value : entropyData) {
        seed ^= value;
    }
    // Seed our main random generator
    randomGen.seed(seed);

}
