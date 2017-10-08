class Leveldb < Formula
  desc "Key-value storage library with ordered mapping"
  homepage "https://github.com/google/leveldb/"
  url "https://s3.eu-central-1.amazonaws.com/wb-rd/leveldb-1.19-macos1012fix.tar.gz"
  sha256 "7a580e543a3b69efb4407619c48dbeac2043eccd385574f9de53f2dcfea65897"

  option "with-test", "Verify the build with make check"

  depends_on "gperftools"
  depends_on "snappy"

  def install
    system "make"
    system "make", "check" if build.bottle? || build.with?("test")

    include.install "include/leveldb"
    bin.install "out-static/leveldbutil"
    lib.install "out-static/libleveldb.a"
    lib.install "out-shared/libleveldb.dylib.1.19" => "libleveldb.1.19.dylib"
    lib.install_symlink lib/"libleveldb.1.19.dylib" => "libleveldb.dylib"
    lib.install_symlink lib/"libleveldb.1.19.dylib" => "libleveldb.1.dylib"
    system "install_name_tool", "-id", "#{lib}/libleveldb.1.dylib", "#{lib}/libleveldb.1.19.dylib"
  end

  test do
    assert_match "dump files", shell_output("#{bin}/leveldbutil 2>&1", 1)
  end
end
