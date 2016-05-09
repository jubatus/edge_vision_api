
def check_detect(file)
  ret = `./build/extract_test #{file}`
  if /detected/ =~ ret
    true
  else
    false
  end
end

dirs = `ls`.split("\n").grep(/^label_/)
dirs.each{|dir|
  jpegs = `ls #{dir}`.split("\n").grep(/jpeg$/)
  jpegs.each{|jpg|
    filename = "#{dir}/#{jpg}"
    if check_detect(filename)
      puts "#{filename} detected!"
    else
      File.delete(filename)
      puts "#{filename} not. so removed."
    end
  }
}
