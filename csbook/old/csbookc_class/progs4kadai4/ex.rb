# �X�щ΍Ђ̎���
# ex.rb

# �����̐ݒ�F�Œ�p�����[�^
n = 50;  nt = (2*n+1)**2;  limit = nt * 0.001 # �S�Ă̊ (0.1%)

# �����p�p�����[�^
ex = 30;  b = 5;  p = 10;  time = 100;  seed = 1

# ����
   bcount = 0
   for i in (1..ex)
      result = `./fire.exe #{n} #{p} #{b} #{time} #{seed}`
      r1, r2, r3 = result.chomp.split(/\s*,\s*/)
      t  = r1.to_i
      nb = r2.to_i
      nu = r3.to_i
      if nu <= limit
         bcount = bcount + 1
      end
      print(t, ", ", nu, ": ", bcount, "\n")
      seed = seed + 1;  i = i + 1
   end
   R = bcount.to_f / ex.to_f
   print(p, ", ", R, "\n")
