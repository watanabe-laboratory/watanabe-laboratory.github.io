# dist1.rb
# �S���_�ԍŒZ�����̌v�Z

### �e��T�u���[�`���̒�` #############
# min �������������߂�T�u���[�`��
# -1 �́u������v�Ƃ��ē��ʈ�������
def min(x, y)
   if x == -1
      return(y)
   elsif y == -1
      return(x)
   else
      if x < y
         return(x)
      else
         return(y)
      end
   end
end

# ��ӂ̘a�����߂�T�u���[�`��
# -1 �́u������v�Ƃ��ē��ʈ�������
def plus(x, y)
   if x == -1 || y == -1
      return(-1)
   else
      return(x + y)
   end
end

# �s��̏o�͂̂��߂̃T�u���[�`��
def printmatrix(n, d)
   for i in 0..n-1
      for j in 0..n-1
         printf("%3d, ", d[i][j])
      end
      print("\n")
   end
   print("\n")
end

# �����s��̂��߂̍s��όv�Z�T�u���[�`��
# U * V
def mult(n, u, v)
   w = Array.new(n){ Array.new(n) }  # �v�Z�r�����ʕێ��p�̔z��
   for i in 0..n-1
      for j in 0..n-1
         tmp = -1
         for k in 0..n-1
            x = plus(u[i][k], v[k][j])
            tmp = min(tmp, x)
         end
         w[i][j] = tmp
      end
   end
   return(w)
end

############################

# �T�C�Y n �̐ݒ�Ə���
n = gets().to_i
a = Array.new(n){ Array.new(n) }
d = Array.new(n){ Array.new(n) }

# �אڋ����s�����
for i in 0..n-1
   a[i] = gets().split.map(&:to_i)
end
printmatrix(n, a)  # ���̓f�[�^�o��

# �s�� d �̌v�Z
# 1 �X�e�b�v��
d = a

# �J��Ԃ�
t = 1
while t < n
   t = t + 1
   # t �X�e�b�v�ڂ̌v�Z
   d = mult(n, d, a)    # D = D * A
end

# �����̏o��
print("result\n")
printmatrix(n, d)
