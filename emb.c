#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Include for string functions

void embedJavaScript(const char *jsFile, const char *svgFile) {
    FILE *jsFp = fopen(jsFile, "r");
    if (!jsFp) {
        perror("Failed to open JavaScript file");
        return;
    }

    // Read JavaScript content
    fseek(jsFp, 0, SEEK_END);
    long jsSize = ftell(jsFp);
    fseek(jsFp, 0, SEEK_SET);
    
    char *jsContent = malloc(jsSize + 1);
    fread(jsContent, 1, jsSize, jsFp);
    jsContent[jsSize] = '\0'; // Null-terminate the string
    fclose(jsFp);

    // Open SVG file for modification
    FILE *svgFp = fopen(svgFile, "r");
    if (!svgFp) {
        perror("Failed to open SVG file");
        free(jsContent);
        return;
    }

    // Read SVG content
    fseek(svgFp, 0, SEEK_END);
    long svgSize = ftell(svgFp);
    fseek(svgFp, 0, SEEK_SET);
    
    char *svgContent = malloc(svgSize + 1);
    fread(svgContent, 1, svgSize, svgFp);
    svgContent[svgSize] = '\0'; // Null-terminate the string
    fclose(svgFp);

    // Find the position to insert the JavaScript
    const char *insertPos = "</svg>"; // Insert before closing tag
    char *newSvgContent = malloc(svgSize + jsSize + 100); // Extra space for JS and null terminator

    // Copy content up to the insertion point
    char *insertPoint = strstr(svgContent, insertPos);
    if (insertPoint) {
        size_t prefixLength = insertPoint - svgContent;
        memcpy(newSvgContent, svgContent, prefixLength);
        
        // Create the new SVG content with embedded JS
        sprintf(newSvgContent + prefixLength, "<script type=\"text/javascript\">\n%s\n</script>\n%s", jsContent, insertPos);
        
        // Write the new SVG content back to the file
        FILE *outFp = fopen(svgFile, "w");
        if (outFp) {
            fwrite(newSvgContent, 1, prefixLength + jsSize + strlen(insertPos) + 100, outFp);
            fclose(outFp);
        } else {
            perror("Failed to open SVG file for writing");
        }
    } else {
        fprintf(stderr, "Closing SVG tag not found.\n");
    }

    // Clean up
    free(jsContent);
    free(svgContent);
    free(newSvgContent);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <js_file> <svg_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    embedJavaScript(argv[1], argv[2]);
    return EXIT_SUCCESS;
}
