# O template diz que tipo que saída deseja-se criar
# Ex.: poderia ser "lib", caso fosse uma biblioteca
TEMPLATE = app
TARGET = ImageViewer

# QT += core
# ---inputs---
HEADERS += src/TerraLibPro/Inpaint/Inpaint.h \
    src/Util/Utilities.h \
    src/TerraLibPro/StructElements/StructElement.h \
    src/TerraLibPro/MorphologicalOperations/MorphologicalOperation.h \
    src/Facade/FacilitiesFacade.h \
    src/TerraLibPro/RegionDetection/RegionDetection.h \
    src/TerraLibPro/Formula/Formula.h \
    src/QTPro/ImageViewer.h \
    src/QTPro/dialogs/DecomposeDialog.h \
    src/QTPro/dialogs/InpaintDialog.h \
    src/QTPro/dialogs/MedianFilterDialog.h \
    src/QTPro/dialogs/MorphologicalOperationDialog.h \
    src/QTPro/dialogs/SepRegionsDialog.h \
    src/QTPro/dialogs/TextureSynthesisDialog.h \
    src/QTPro/dialogs/abstractDialog/CustomDialog.h
SOURCES += src/TerraLibPro/Inpaint/Inpaint.cpp \
    src/Util/Utilities.cpp \
    src/TerraLibPro/StructElements/StructElement.cpp \
    src/TerraLibPro/MorphologicalOperations/MorphologicalOperation.cpp \
    src/Facade/FacilitiesFacade.cpp \
    src/TerraLibPro/RegionDetection/RegionDetection.cpp \
    src/TerraLibPro/Formula/Formula.cpp \
    src/QTPro/ImageViewer.cpp \
    src/QTPro/dialogs/DecomposeDialog.cpp \
    src/QTPro/dialogs/InpaintDialog.cpp \
    src/QTPro/dialogs/MedianFilterDialog.cpp \
    src/QTPro/dialogs/MorphologicalOperationDialog.cpp \
    src/QTPro/dialogs/SepRegionsDialog.cpp \
    src/QTPro/dialogs/TextureSynthesisDialog.cpp \
    src/QTPro/dialogs/abstractDialog/CustomDialog.cpp \
    main.cpp
INCLUDEPATH += /home/10088000201/TerraLib/src/terralib/kernel \
    /home/10088000201/TerraLib/src/terralib/image_processing \
    /home/10088000201/TerraLib/src/terralib/image_processing/data_structs
LIBS += -L"/home/10088000201/Laps/bibliotecas_externas" \
    -lterralib \
    -lz \
    -lpng \
    -lfreetype \
    -ljpeg \
    -ltiff \
    -lterralibpdi \
    -lfftw3

# Também é necessário configurar a variável LD_LIBRARY_PATH no "Run as configuration"
FORMS += 
RESOURCES += iconResources.qrc
