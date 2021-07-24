import java.util.HashMap;

public class mappy {
    public static void main(String[] args) {
        HashMap<Long, Long> map = new HashMap();

        long total = 0;
        for (long i = 0; i < 20000000; i++) {
            Long x = i;
            map.put(x, x);
            Long x2 = map.get(x);
            total+=x2.hashCode();
        }

        for (long i = 0; i < 20000000; i++) {
            Long x = i;
            Long x2 = map.get(x);
            map.remove(x);
        }
        System.out.println(total);
    }
}