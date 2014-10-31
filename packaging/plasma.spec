# 
# Do NOT Edit the Auto-generated Part!
# Generated by: spectacle version 0.27
# 

Name:       plasma

# >> macros
# << macros

Summary:    KDE Frameworks 5 Tier 3 framework with Plasma 2 libraries and runtime component
Version:    5.3.0
Release:    3
Group:      System/Base
License:    GPLv2+
URL:        http://www.kde.org
Source0:    %{name}-%{version}.tar.xz
Source100:  plasma.yaml
Source101:  plasma-rpmlintrc
Patch0:     0001-install-plasma-wallpaper.desktop-in-servicetypes.patch
Requires:   kf5-filesystem
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5DBus)
BuildRequires:  pkgconfig(Qt5Xml)
BuildRequires:  pkgconfig(Qt5Network)
BuildRequires:  pkgconfig(Qt5Gui)
BuildRequires:  pkgconfig(Qt5Widgets)
BuildRequires:  pkgconfig(Qt5Concurrent)
BuildRequires:  pkgconfig(Qt5Test)
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(Qt5Svg)
BuildRequires:  pkgconfig(Qt5Script)
BuildRequires:  pkgconfig(Qt5Quick)
BuildRequires:  pkgconfig(Qt5X11Extras)
BuildRequires:  pkgconfig(x11)
BuildRequires:  pkgconfig(x11-xcb)
BuildRequires:  pkgconfig(xcb)
BuildRequires:  pkgconfig(xcb-composite)
BuildRequires:  pkgconfig(xcb-damage)
BuildRequires:  pkgconfig(xcb-shape)
BuildRequires:  pkgconfig(xcb-xfixes)
BuildRequires:  pkgconfig(xcb-render)
BuildRequires:  pkgconfig(xrender)
BuildRequires:  pkgconfig(xscrnsaver)
BuildRequires:  pkgconfig(xext)
BuildRequires:  pkgconfig(sm)
BuildRequires:  pkgconfig(openssl)
BuildRequires:  kf5-rpm-macros
BuildRequires:  extra-cmake-modules
BuildRequires:  qt5-tools
BuildRequires:  kactivities-libs-devel
BuildRequires:  karchive-devel
BuildRequires:  kconfigwidgets-devel
BuildRequires:  kcoreaddons-devel
BuildRequires:  kdbusaddons-devel
BuildRequires:  kdeclarative-devel
BuildRequires:  kglobalaccel-devel
BuildRequires:  kguiaddons-devel
BuildRequires:  ki18n-devel
BuildRequires:  kiconthemes-devel
BuildRequires:  kio-devel
BuildRequires:  kservice-devel
BuildRequires:  kwindowsystem-devel
BuildRequires:  kxmlgui-devel
BuildRequires:  kdnssd-devel
BuildRequires:  kdoctools-devel
BuildRequires:  libGL-devel

%description
KDE Frameworks 5 Tier 3 framework with Plasma 2 libraries and runtime component


%package devel
Summary:    Development files for %{name}
Group:      Development/Libraries
Requires:   %{name} = %{version}-%{release}
Requires:   extra-cmake-modules
Requires:   kactivities-libs-devel
Requires:   karchive-devel
Requires:   kconfigwidgets-devel
Requires:   kcoreaddons-devel
Requires:   kdbusaddons-devel
Requires:   kdeclarative-devel
Requires:   kglobalaccel-devel
Requires:   kguiaddons-devel
Requires:   ki18n-devel
Requires:   kiconthemes-devel
Requires:   kio-devel
Requires:   kservice-devel
Requires:   kwindowsystem-devel
Requires:   kxmlgui-devel
Requires:   kdoctools-devel

%description devel
The %{name}-devel package contains the files necessary to develop applications
that use %{name}.


%prep
%setup -q -n %{name}-%{version}

# 0001-install-plasma-wallpaper.desktop-in-servicetypes.patch
%patch0 -p1
# >> setup
# << setup

%build
# >> build pre
%kf5_make
# << build pre



# >> build post
# << build post

%install
rm -rf %{buildroot}
# >> install pre
%kf5_make_install
# << install pre

# >> install post
# << install post

%find_lang plasma5_qt --with-qt --all-name || :

%files -f plasma5_qt.lang
%defattr(-,root,root,-)
%doc COPYING.LIB README.md
%{_kf5_bindir}/dpitest
%{_kf5_bindir}/plasmapkg2
%{_kf5_libdir}/libKF5Plasma.so.*
%{_kf5_libdir}/libKF5PlasmaQuick.so.*
%{_qt5_libdir}/qt5/platformqml/touch/org/kde/plasma
%{_kf5_qmldir}/org/kde/*
%{_kf5_plugindir}/*
%{_kf5_dbusinterfacesdir}/*.xml
%{_kf5_sharedir}/plasma/
%{_kf5_servicesdir}/*
%{_kf5_servicetypesdir}/*
%{_mandir}/man1/plasmapkg2.1.gz
# >> files
# << files

%files devel
%defattr(-,root,root,-)
%{_kf5_cmakedir}/KF5Plasma
%{_kf5_cmakedir}/KF5PlasmaQuick
%{_kf5_libdir}/libKF5Plasma.so
%{_kf5_libdir}/libKF5PlasmaQuick.so
%{_kf5_includedir}/plasma_version.h
%{_kf5_includedir}/plasma/
%{_kf5_includedir}/Plasma/
# >> files devel
# << files devel
