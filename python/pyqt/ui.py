# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'myqt.ui'
#
# Created: Sat Mar  2 21:02:29 2013
#      by: PyQt4 UI code generator 4.9.6
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    def _fromUtf8(s):
        return s

try:
    _encoding = QtGui.QApplication.UnicodeUTF8
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig, _encoding)
except AttributeError:
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig)

class Ui_Tasker(object):
    def setupUi(self, Tasker):
        Tasker.setObjectName(_fromUtf8("Tasker"))
        Tasker.resize(800, 600)
        self.centralwidget = QtGui.QWidget(Tasker)
        self.centralwidget.setObjectName(_fromUtf8("centralwidget"))
        self.ksignalplotter = KSignalPlotter(self.centralwidget)
        self.ksignalplotter.setGeometry(QtCore.QRect(230, 100, 200, 200))
        self.ksignalplotter.setObjectName(_fromUtf8("ksignalplotter"))
        Tasker.setCentralWidget(self.centralwidget)
        self.menubar = QtGui.QMenuBar(Tasker)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 800, 21))
        self.menubar.setObjectName(_fromUtf8("menubar"))
        self.menuTest = QtGui.QMenu(self.menubar)
        self.menuTest.setObjectName(_fromUtf8("menuTest"))
        Tasker.setMenuBar(self.menubar)
        self.statusbar = QtGui.QStatusBar(Tasker)
        self.statusbar.setObjectName(_fromUtf8("statusbar"))
        Tasker.setStatusBar(self.statusbar)
        self.menubar.addAction(self.menuTest.menuAction())

        self.retranslateUi(Tasker)
        QtCore.QMetaObject.connectSlotsByName(Tasker)

    def retranslateUi(self, Tasker):
        Tasker.setWindowTitle(_translate("Tasker", "MyQt", None))
        self.menuTest.setTitle(_translate("Tasker", "Test", None))

from ksignalplotter import KSignalPlotter
