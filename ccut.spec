Name:           ccut
Version:        3.1
Release:        1%{?dist}
Summary:        ccut is a unix 'cut' command with a couple of extra features

License:        GPLv3
URL:            https://github.com/ColumPaget/ColumsCut/
Source0:        https://github.com/ColumPaget/ColumsCut/archive/v3.1.tar.gz

BuildRequires:  gcc

%description
ccut is a unix 'cut' command with a couple of extra features (and
a couple of features missing).

ccut supports:

  * utf-8 input in -c mode multiple delimiters using a string as
    the delimiter rather than a character treating runs of a
    delimiter as one delimiter
  * quoting using either quotes or backslash within the cut
    document (for example, honor " quotes or , in a csv) honoring
    quotes or backslash in a document but stripping them from
    output outputting fields in any specified order
  * outputting a different delimiter than those within the cut
    document
  * setting variables in the shell to values of cut fields

%prep
%autosetup -p1 -n ColumsCut-%{version}
# Typo in the filename.
if [ -f LICENCE ]; then
mv LICENCE LICENSE
fi

%build
%make_build FLAGS="%{optflags}"

%install
%make_install PREFIX=%{_prefix}

%files
%license LICENSE
%doc README.md
%{_bindir}/ccut
%{_mandir}/man1/ccut.1*

%changelog
* Thu Nov 19 2020 Joel Barrios <http://www.alcancelibre.org/> - 3.1-1
- Initial spec file.
