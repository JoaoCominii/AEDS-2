import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.Scanner;


class Show {

    private String Show_ID;
    private String Type;
    private String Title;
    private String Director;
    private String[] Cast;
    private String Country;
    private String Date_Added;
    private int Release_Year;
    private String Rating;
    private String Duration;
    private String[] Listed_In;

    // Construtor padrao
    public Show() {
        this.Show_ID = "NaN";
        this.Type = "NaN";
        this.Title = "NaN";
        this.Director = "NaN";
        this.Cast = new String[0];
        this.Country = "NaN";
        this.Date_Added = "NaN";
        this.Release_Year = 0;
        this.Rating = "NaN";
        this.Duration = "NaN";
        this.Listed_In = new String[0];
    }

    // Construtor com parametros
    public Show(String Show_ID, String Type, String Title, String Director, String[] Cast, String Country, String date_Added2, int Release_Year, String Rating, String Duration, String[] Listed_In) {
        this.Show_ID = Show_ID != null ? Show_ID : "NaN";
        this.Type = Type != null ? Type : "NaN";
        this.Title = Title != null ? Title : "NaN";
        this.Director = Director != null ? Director : "NaN";
        this.Cast = Cast != null ? Cast : new String[0];
        this.Country = Country != null ? Country : "NaN";
        this.Date_Added = date_Added2 != null ? date_Added2 : "NaN";
        this.Release_Year = Release_Year > 0 ? Release_Year : 0;
        this.Rating = Rating != null ? Rating : "NaN";
        this.Duration = Duration != null ? Duration : "NaN";
        this.Listed_In = Listed_In != null ? Listed_In : new String[0];
        ordenarBubbleSort(this.Cast);
        ordenarBubbleSort(this.Listed_In);
    }

    // Getters e Setters
    public String getShow_ID() {
        return Show_ID;
    }

    public void setShow_ID(String Show_ID) {
        this.Show_ID = Show_ID;
    }

    public String getType() {
        return Type;
    }

    public void setType(String Type) {
        this.Type = Type;
    }

    public String getTitle() {
        return Title;
    }

    public void setTitle(String Title) {
        this.Title = Title;
    }

    public String getDirector() {
        return Director;
    }

    public void setDirector(String Director) {
        this.Director = Director;
    }

    public String[] getCast() {
        return Cast;
    }

    public void setCast(String[] Cast) {
        this.Cast = Cast;
        ordenarBubbleSort(this.Cast);
    }

    public String getCountry() {
        return Country;
    }

    public void setCountry(String Country) {
        this.Country = Country;
    }

    public String getDate_Added() {
        return Date_Added;
    }

    public void setDate_Added(String Date_Added) {
        this.Date_Added = Date_Added;
    }

    public int getRelease_Year() {
        return Release_Year;
    }

    public void setRelease_Year(int Release_Year) {
        this.Release_Year = Release_Year;
    }

    public String getRating() {
        return Rating;
    }

    public void setRating(String Rating) {
        this.Rating = Rating;
    }

    public String getDuration() {
        return Duration;
    }

    public void setDuration(String Duration) {
        this.Duration = Duration;
    }

    public String[] getListed_In() {
        return Listed_In;
    }

    public void setListed_In(String[] Listed_In) {
        this.Listed_In = Listed_In;
        ordenarBubbleSort(this.Listed_In);
    }

    public String imprimir() {
        // Remove os espaços de cada elemento do array Cast e Listed_In
        String castFormatado = "[" + String.join(", ", removerEspacos(this.Cast)) + "]";
        String listedInFormatado = "[" + String.join(", ", removerEspacos(this.Listed_In)) + "]";
    
        // Retorna a string formatada com os arrays, mesmo quando estão vazios
        return ("=> " + getShow_ID().trim() + " ## " + getType().trim() + " ## " + getTitle().trim() + " ## " + getDirector().trim() + " ## "
                + castFormatado + " ## " + getCountry().trim() + " ## " + Date_Added.trim()
                + " ## " + getRelease_Year() + " ## " + getRating().trim() + " ## " + getDuration().trim() + " ## "
                + listedInFormatado + " ##");
    }
    
    // Método auxiliar para remover espaços dos elementos de um array de strings
    private String[] removerEspacos(String[] array) {
        String[] resultado = new String[array.length];
        for (int i = 0; i < array.length; i++) {
            resultado[i] = array[i].trim(); // Remove espaços do início e do final de cada string
        }
        return resultado;
    }
    

    // metodo clone
    public String clone () {
        Show cloneShow = new Show(this.Show_ID, this.Type, this.Title, this.Director, this.Cast, this.Country, this.Date_Added, this.Release_Year, this.Rating, this.Duration, this.Listed_In);
        return cloneShow.imprimir();
    }

    // Método para ler os dados do arquivo CSV
    public static Show[] Ler() {
        String caminhoArquivo = "/tmp/DisneyPlus.csv";
        Show[] shows = new Show[1368]; // Definindo tamanho inicial do array
        int contador = 0;

        try (BufferedReader br = new BufferedReader(new FileReader(caminhoArquivo))) {
            String linha;
            boolean primeiraLinha = true;


            while ((linha = br.readLine()) != null && contador < 1368) {
                if (primeiraLinha) { // Ignora cabeçalho
                    primeiraLinha = false;
                    continue;
                }

                // Usa regex para separar os valores corretamente, considerando aspas
                String[] valores = linha.split(",(?=(?:[^\"]*\"[^\"]*\")*[^\"]*$)");

                // Tratamento de valores ausentes
                for (int i = 0; i < valores.length; i++) {
                    valores[i] = valores[i].replaceAll("^\"|\"$", "").trim(); // Remove aspas extras
                    if (valores[i].isEmpty()) {
                        valores[i] = "NaN"; // Substitui valores vazios por "NaN"
                    }
                }

                // Separa e cria um objeto Show
                String[] cast = valores[4].split(",");
                String[] listedIn = valores[10].split(",");
                Show show = new Show(
                    valores[0], // Show_ID
                    valores[1], // Type
                    valores[2], // Title
                    valores[3], // Director
                    cast, // Cast
                    valores[5], // Country
                    valores[6], // Date_Added
                    Integer.parseInt(valores[7]), // Release_Year
                    valores[8], // Rating
                    valores[9], // Duration
                    listedIn // Listed_In
                );

                // Adiciona o objeto no array
                shows[contador++] = show;
            }
        }catch (IOException e) {
            System.err.println("Erro ao ler o arquivo: " + e.getMessage());
        } catch (NumberFormatException e) {
            System.err.println("Erro ao converter ano de lançamento: " + e.getMessage());
        }

        return shows;
    }



// Método para ordenação manual usando Bubble Sort
private void ordenarBubbleSort(String[] array) {
    int n = array.length;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            // Remove espaços e força para minúsculas para comparação
            String atual = array[j].trim().toLowerCase();
            String proximo = array[j + 1].trim().toLowerCase();

            if (atual.compareTo(proximo) > 0) { // Verifica a ordem alfabética
                // Troca os elementos
                String temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
}
}


public class TP02Q1 {
    public static void main(String[] args) {

        // Carrega todos os Shows do arquivo CSV
        Show[] todosShows = Show.Ler();

        // Array para armazenar os 300 Shows
        Show[] showsSelecionados = new Show[300];
        int contador = 0;

        // Usa Scanner para ler os IDs do teclado
        Scanner scanner = new Scanner(System.in);
        while (scanner.hasNextLine()) {
            String id = scanner.nextLine();
            if (id.equals("FIM")) { // Finaliza ao receber "FIM"
                break;
            }

            // Busca o Show com o ID fornecido
            for (Show show : todosShows) {
                if (show != null && show.getShow_ID().equals(id)) {
                    showsSelecionados[contador++] = show;
                    break;
                }
            }
        }
        scanner.close(); // Fecha o Scanner

        // Exibe os Shows selecionados usando o método imprimir
        for (int i = 0; i < contador; i++) {
            System.out.println(showsSelecionados[i].imprimir());
        }
    }
}