# List of all the board related files.
BOARDSRC = $(PROJECTDIR)/board/board.c

# Required include directories
BOARDINC = $(PROJECTDIR)/board

# Shared variables
ALLCSRC += $(BOARDSRC)
ALLINC  += $(BOARDINC)
