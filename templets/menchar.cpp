#include <vector>
#include <string>
#include <algorithm>

class Manacher {
public:
    std::vector<int> palindromeLengths;
    std::string transformedString;

    void preprocess(const std::string &input) {
        transformedString = "#";
        for (char ch : input) {
            transformedString += ch;
            transformedString += '#';
        }
        
        int n = transformedString.size();
        palindromeLengths.resize(n, 0);
        
        int center = 0, rightBoundary = 0;
        for (int i = 0; i < n; ++i) {
            // Use the mirror property to initialize the palindrome length
            if (i < rightBoundary) {
                palindromeLengths[i] = std::min(rightBoundary - i, palindromeLengths[center * 2 - i]);
            } else {
                palindromeLengths[i] = 1; // At least the character itself
            }

            // Expand around the center
            while (i - palindromeLengths[i] >= 0 && 
                   i + palindromeLengths[i] < n && 
                   transformedString[i + palindromeLengths[i]] == transformedString[i - palindromeLengths[i]]) {
                palindromeLengths[i]++;
            }

            // Update center and right boundary if we've extended past the current known right boundary
            if (i + palindromeLengths[i] > rightBoundary) {
                center = i;
                rightBoundary = i + palindromeLengths[i];
            }
        }
    }

    int getLongestPalindromeLength(int index, bool isOddLength = true) const {
        int adjustedIndex = 2 * index + (isOddLength ? 1 : 0);
        return palindromeLengths[adjustedIndex] - 1; // Exclude the added '#' characters
    }

    bool isPalindromeInRange(int left, int right) const {
        int palindromeCenter = (left + right) / 2;
        return (right - left + 1) <= getLongestPalindromeLength(palindromeCenter, (left % 2 == right % 2));
    }

    std::string longestPalindromeSubstring() {
        int maxLength = 0, startIndex = 0;

        for (int i = 0; i < transformedString.size(); ++i) {
            for (bool isOdd : {true, false}) {
                int length = getLongestPalindromeLength(i, isOdd);
                if (length > maxLength) {
                    maxLength = length;
                    startIndex = i;
                }
            }
        }

        int substringStart = startIndex - maxLength / 2;
        if (maxLength % 2 == 0) {
            substringStart += 1; // Adjust for even length
        }

        return transformedString.substr(substringStart, maxLength);
    }
};
