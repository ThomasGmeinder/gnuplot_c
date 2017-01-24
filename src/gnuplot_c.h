// Gnuplot/C interface library header file
// Please ensure that the system path includes an entry for the gnuplot binary folder

#if (!GPC_H)

#define GPC_H                   1                   // Gnuplot/C included

#ifndef GPC_DEBUG
  #define GPC_DEBUG             0                   // Set to '1' to enable Gnuplot text debug output, '0' otherwise
#endif

#include<stdlib.h>
#include <sys/stat.h>

#define MAX_NUM_GRAPHS          50                  // Maximum number of graphs (multiplots) in a plot

#define GPC_AUTO_SCALE          0.0                 // Auto scaling
#define GPC_IMG_AUTO_SCALE      0                   // Auto scaling for images

#define CANVAS_WIDTH            800
#define CANVAS_HEIGHT           600
#define PLOT_LMARGIN            (80.0/CANVAS_WIDTH)   // 512 pixel X axis
#define PLOT_RMARGIN            (591.0/CANVAS_WIDTH)

#define GPC_NO_ERROR            0                   // No error
#define GPC_ERROR               1                   // Error

#define GPC_FALSE               0                   // False flag
#define GPC_TRUE                1                   // True flag

#define GPC_END_PLOT            NULL                // Flag to indicate end of the plot

enum gpcMultiFastMode                               // Multiplot / fast plot modes
{
    GPC_MULTIPLOT = 0,
    GPC_FASTPLOT
};

enum gpcKeyMode                                     // Legend / Key mode
{
    GPC_KEY_DISABLE = 0,
    GPC_KEY_ENABLE
};

enum gpcPlotSignMode                                // Sign modes - signed, positive, negative
{
    GPC_SIGNED = 0,
    GPC_POSITIVE,
    GPC_NEGATIVE
};

enum gpcNewAddGraphMode                             // New / Add graph modes
{
    GPC_NEW = 0,
    GPC_ADD
};

enum gpcPoleZeroMode                                // Pole zero plot modes
{
    GPC_COMPLEX_POLE = 0,
    GPC_CONJUGATE_POLE,
    GPC_COMPLEX_ZERO,
    GPC_CONJUGATE_ZERO
};

                                                    // Spectrogram and image colour palettes
#define GPC_MONOCHROME "set palette defined (0 'black', 1 'white')"
#define GPC_COLOUR "set palette defined (0 'black', 1 'blue', 2 'red', 3 'yellow', 4 'white')"


typedef struct                                      // Complex data type
{
    double  real;
    double  imag;
} ComplexRect_s;


typedef struct
{
    char filename [40];                             // Graph filename
    char title [80];                                // Graph title
    char formatString [40];                         // Graph format string "lines", "points" etc
} h_GPC_Graph;


typedef struct
{
    FILE *pipe;                                     // Pipe to Gnuplot instance
    int highestGraphNumber;                         // Number of graphs on this plot
    h_GPC_Graph graphArray [MAX_NUM_GRAPHS];        // Array of graphs
    char plotTitle[80];                             // Plot title
    int filenameRootId;                             // Multiplot filename root Id
    enum gpcMultiFastMode multiFastMode;            // Multiplot / fast plot mode
    double scalingMode;                             // Scaling mode / dimension for XY and PZ graphs
    long xAxisLength;                               // X axis length for spectrogram plots and images
    long yAxisLength;                               // Y axis length for spectrogram plots and images
    double xMin;                                    // Minimum value of X axis - used for axis labels
    double xMax;                                    // Maximum value of X axis - used for axis labels
    double yMin;                                    // Minimum value of Y axis - used for axis labels
    double yMax;                                    // Maximum value of Y axis - used for axis labels
    long numberOf2ndAxisPlotted;                    // Number of columns for spectrogram plot
    int tempFilesUsedFlag;                          // Flag set when temporary files used
} h_GPC_Plot;


h_GPC_Plot *gpc_init_2d (const char *plotTitle,     // Plot title
    const char *xLabel,                             // X axis label
    const char *yLabel,                             // Y axis label
    const double scalingMode,                       // Scaling mode
    const enum gpcPlotSignMode signMode,            // Sign mode - signed, positive, negative
    const enum gpcMultiFastMode multiFastMode,      // Multiplot / fast plot mode
    const enum gpcKeyMode keyMode);                 // Legend / key mode

h_GPC_Plot *gpc_init_2d_logscalex (const char *plotTitle,   // Plot title
    const char *xLabel,                             // X axis label
    const char *yLabel,                             // Y axis label
    const double scalingMode,                       // Scaling mode
    const enum gpcPlotSignMode signMode,            // Sign mode - signed, positive, negative
    const enum gpcMultiFastMode multiFastMode,      // Multiplot / fast plot mode
    const enum gpcKeyMode keyMode);                 // Legend / key mode
    
int gpc_plot_2d (h_GPC_Plot *plotHandle,            // Plot handle
    const double *pData,                            // Dataset pointer
    const int graphLength,                          // Dataset length
    const char *pDataName,                          // Dataset title
    const double xMin,                              // Minimum X value
    const double xMax,                              // Maximum X value
    const char *plotType,                           // Plot type - "lines", "points", "impulses", "linespoints"
    const char *pColour,                            // Colour - Use gnuplot> show colornames to see available colours
    const enum gpcNewAddGraphMode addMode);         // Add / new mode
    
h_GPC_Plot *gpc_init_xy (const char *plotTitle,     // Plot title
    const char *xLabel,                             // X axis label
    const char *yLabel,                             // Y axis label
    const double dimension,                         // Dimension - this is square
    const enum gpcKeyMode keyMode);                 // Legend / key mode
    
int gpc_plot_xy (h_GPC_Plot *plotHandle,            // Plot handle
    const ComplexRect_s *pData,                     // Dataset pointer
    const int graphLength,                          // Dataset length
    const char *pDataName,                          // Dataset title
    const char *plotType,                           // Plot type - "lines", "points", "impulses", "linespoints"
    const char *pColour,                            // Colour - Use gnuplot> show colornames to see available colours
    const enum gpcNewAddGraphMode addMode);         // Add / new mode
    
h_GPC_Plot *gpc_init_pz (const char *plotTitle,     // Plot title
    const double dimension,                         // Dimension - this is square
    const enum gpcKeyMode keyMode);                 // Legend / key mode
    
int gpc_plot_pz (h_GPC_Plot *plotHandle,            // Plot handle
    const ComplexRect_s *pData,                     // Dataset pointer
    const int graphLength,                          // Dataset length
    const char *pDataName,                          // Dataset title
    const enum gpcPoleZeroMode poleZeroMode,        // Pole-zero mode
    const enum gpcNewAddGraphMode addMode);         // Add / new mode

h_GPC_Plot *gpc_init_spectrogram (const char *plotTitle,  // Plot title
    const char *xLabel,                             // X axis label
    const char *yLabel,                             // Y axis label
    const int xAxisLength,                          // X axis length
    const int yAxisLength,                          // Y axis length
    const double yMin,                              // Minimum Y value
    const double yMax,                              // Maximum Y value
    const double zMin,                              // Minimum Z value
    const double zMax,                              // Maximum Z value
    const char *colourPalette,                      // Colour palette
    const enum gpcKeyMode keyMode);                 // Legend / key mode

int gpc_plot_spectrogram (h_GPC_Plot *plotHandle,   // Plot handle
    const double *pData,                            // Dataset pointer
    const char *pDataName,                          // Dataset title
    const double xMin,                              // Minimum X value
    const double xMax);                             // Maximum X value

h_GPC_Plot *gpc_init_image (const char *plotTitle,  // Plot title
    const int xAxisLength,                          // X axis length
    const int yAxisLength,                          // X axis length
    const unsigned int zMin,                        // Minimum Z value
    const unsigned int zMax,                        // Maximum Z value
    const char *colourPalette,                      // Colour palette
    const enum gpcKeyMode keyMode);                 // Legend / key mode

int gpc_plot_image (h_GPC_Plot *plotHandle,         // Plot handle
    const unsigned char *pData,                     // Dataset pointer
    const char *pDataName);                         // Dataset title

h_GPC_Plot *gpc_init_polar (const char *plotTitle,  // Plot title
    const double gMin,                              // Minimum gain value
    const double gMax,                              // Maximum gain value
    const enum gpcKeyMode keyMode);                 // Legend / key mode

int gpc_plot_polar (h_GPC_Plot *plotHandle,         // Plot handle
    const double *pAngles,                          // Angles dataset pointer
    const double *pGains,                           // Gains dataset pointer
    const int graphLength,                          // Dataset length
    const char *pDataName,                          // Dataset title
    const char *plotType,                           // Plot type - "lines", "points", "impulses", "linespoints"
    const char *pColour,                            // Colour - Use gnuplot> show colornames to see available colours
    const enum gpcNewAddGraphMode addMode);         // Add / new mode

void gpc_close (h_GPC_Plot *plotHandle);            // Plot handle

#endif    // End of GPC_H