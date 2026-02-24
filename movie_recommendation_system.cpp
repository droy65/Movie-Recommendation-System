#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <queue>
#include <stack>
#include <algorithm>
#include <climits>
#include <cstdlib>
#include <cmath>

using namespace std;

// Movie class to store movie information
class Movie {
public:
    int id;
    string title;
    string genre;
    string actor;
    double rating;
    string industry; // "Hollywood" or "Bollywood"
    
    Movie() : id(0), title(""), genre(""), actor(""), rating(0.0), industry("") {}
    
    Movie(int id, string title, string genre, string actor, double rating, string industry) 
        : id(id), title(title), genre(genre), actor(actor), rating(rating), industry(industry) {}
    
    void display() const {
        cout << title << " (" << genre << ", " << actor << ", Rating: " << rating << ", " << industry << ")";
    }
};

// Comparison function for sorting movies by rating
bool compareByRating(const pair<double, int>& a, const pair<double, int>& b) {
    return a.first > b.first;
}

// Comparison for sorting by similarity
bool compareBySimilarity(const pair<int, double>& a, const pair<int, double>& b) {
    return a.second > b.second;
}

// Comparison for sorting actors by average rating
bool compareActorByRating(const pair<double, string>& a, const pair<double, string>& b) {
    return a.first > b.first;
}

// Content-Based Recommendation System
class ContentBasedRecommender {
private:
    vector<Movie> movies;
    map<string, vector<int> > genreToMovies;
    map<string, vector<int> > actorToMovies;
    map<string, vector<int> > industryToMovies;
    
public:
    void addMovie(const Movie& movie) {
        int index = movies.size();
        movies.push_back(movie);
        
        // Index by genre
        genreToMovies[movie.genre].push_back(index);
        
        // Index by actor
        actorToMovies[movie.actor].push_back(index);
        
        // Index by industry
        industryToMovies[movie.industry].push_back(index);
    }
    
    // Helper function to find movie index by title
    int findMovieIndex(const string& movieTitle) {
        for (int i = 0; i < movies.size(); i++) {
            if (movies[i].title == movieTitle) {
                return i;
            }
        }
        return -1;
    }
    
    // Get all unique genres for a specific industry
    vector<string> getGenresByIndustry(const string& industry) {
        vector<string> genres;
        map<string, vector<int> >::iterator it;
        
        if (industryToMovies.find(industry) == industryToMovies.end()) {
            return genres;
        }
        
        vector<int> industryMovieIndices = industryToMovies[industry];
        map<string, bool> genreExists;
        
        for (int i = 0; i < industryMovieIndices.size(); i++) {
            string genre = movies[industryMovieIndices[i]].genre;
            if (!genreExists[genre]) {
                genreExists[genre] = true;
                genres.push_back(genre);
            }
        }
        
        return genres;
    }
    
    // Get all movies in a genre for a specific industry
    vector<Movie> getMoviesByGenreAndIndustry(const string& genre, const string& industry) {
        vector<Movie> result;
        
        if (genreToMovies.find(genre) == genreToMovies.end()) {
            return result;
        }
        
        vector<int> genreMovies = genreToMovies[genre];
        
        for (int i = 0; i < genreMovies.size(); i++) {
            int idx = genreMovies[i];
            if (movies[idx].industry == industry) {
                result.push_back(movies[idx]);
            }
        }
        
        return result;
    }
    
    // Get all movies in a genre with their original indices for a specific industry
    vector<pair<Movie, int> > getMoviesInGenreWithIndices(const string& genre, const string& industry) {
        vector<pair<Movie, int> > genreMovies;
        
        if (genreToMovies.find(genre) == genreToMovies.end()) {
            return genreMovies;
        }
        
        vector<int> movieIndices = genreToMovies[genre];
        
        for (int i = 0; i < movieIndices.size(); i++) {
            int idx = movieIndices[i];
            if (movies[idx].industry == industry) {
                genreMovies.push_back(make_pair(movies[idx], idx));
            }
        }
        
        return genreMovies;
    }
    
    // Recommend movies by genre and industry (top rated)
    vector<Movie> recommendByGenreAndIndustry(const string& genre, const string& industry, int topN = 5) {
        vector<Movie> recommendations;
        
        if (genreToMovies.find(genre) == genreToMovies.end()) {
            return recommendations;
        }
        
        vector<int> genreMovies = genreToMovies[genre];
        
        // Create a vector of rating-index pairs for sorting
        vector<pair<double, int> > ratingPairs;
        
        for (int i = 0; i < genreMovies.size(); i++) {
            int idx = genreMovies[i];
            if (movies[idx].industry == industry) {
                ratingPairs.push_back(make_pair(movies[idx].rating, idx));
            }
        }
        
        // Sort by rating (highest first)
        sort(ratingPairs.begin(), ratingPairs.end(), compareByRating);
        
        // Take top N
        for (int i = 0; i < min(topN, (int)ratingPairs.size()); i++) {
            recommendations.push_back(movies[ratingPairs[i].second]);
        }
        
        return recommendations;
    }
    
    // Recommend movies by actor
    vector<Movie> recommendByActor(const string& actor, int topN = 5) {
        vector<Movie> recommendations;
        
        if (actorToMovies.find(actor) == actorToMovies.end()) {
            return recommendations;
        }
        
        vector<int> actorMovies = actorToMovies[actor];
        
        // Create a vector of rating-index pairs for sorting
        vector<pair<double, int> > ratingPairs;
        
        for (int i = 0; i < actorMovies.size(); i++) {
            int idx = actorMovies[i];
            ratingPairs.push_back(make_pair(movies[idx].rating, idx));
        }
        
        // Sort by rating (highest first)
        sort(ratingPairs.begin(), ratingPairs.end(), compareByRating);
        
        // Take top N
        for (int i = 0; i < min(topN, (int)ratingPairs.size()); i++) {
            recommendations.push_back(movies[ratingPairs[i].second]);
        }
        
        return recommendations;
    }
    
    int getMovieCount() {
        return movies.size();
    }
    
    string getMovieTitle(int index) {
        if (index >= 0 && index < movies.size()) {
            return movies[index].title;
        }
        return "";
    }
    
    Movie getMovieByIndex(int index) {
        if (index >= 0 && index < movies.size()) {
            return movies[index];
        }
        return Movie();
    }
    
    // Get movie by title
    Movie getMovieByTitle(const string& title) {
        int idx = findMovieIndex(title);
        if (idx != -1) {
            return movies[idx];
        }
        return Movie();
    }
};

// Graph-Based Recommendation System
class GraphBasedRecommender {
private:
    vector<Movie> movies;
    vector<vector<pair<int, double> > > adjList; // adjacency list with similarity scores
    map<string, int> titleToId;
    map<string, vector<int> > genreToMovies;
    
    // Calculate similarity between two movies
    double calculateSimilarity(const Movie& m1, const Movie& m2) {
        double similarity = 0.0;
        
        // Genre similarity (30% weight)
        if (m1.genre == m2.genre) similarity += 0.3;
        
        // Actor similarity (30% weight)
        if (m1.actor == m2.actor) similarity += 0.3;
        
        // Industry similarity (20% weight)
        if (m1.industry == m2.industry) similarity += 0.2;
        
        // Rating similarity (20% weight) - closer ratings mean higher similarity
        double ratingDiff = fabs(m1.rating - m2.rating) / 10.0;
        similarity += 0.2 * (1.0 - ratingDiff);
        
        return similarity;
    }
    
public:
    void addMovie(const Movie& movie) {
        int id = movies.size();
        movies.push_back(movie);
        titleToId[movie.title] = id;
        genreToMovies[movie.genre].push_back(id);
    }
    
    // Build similarity graph between all movies
    void buildSimilarityGraph() {
        adjList.clear();
        adjList.resize(movies.size());
        
        for (int i = 0; i < movies.size(); i++) {
            for (int j = i + 1; j < movies.size(); j++) {
                double similarity = calculateSimilarity(movies[i], movies[j]);
                
                // Add edge only if similarity is above threshold
                if (similarity > 0.2) {
                    adjList[i].push_back(make_pair(j, similarity));
                    adjList[j].push_back(make_pair(i, similarity));
                }
            }
        }
    }
    
    // Get genre-based recommendations using graph similarity for a specific industry
    vector<Movie> recommendByGenreGraph(const string& genre, const string& industry, int topN = 5) {
        vector<Movie> recommendations;
        
        if (genreToMovies.find(genre) == genreToMovies.end()) {
            return recommendations;
        }
        
        vector<int> genreMovies = genreToMovies[genre];
        
        // Filter movies by industry first
        vector<int> industryGenreMovies;
        for (int i = 0; i < genreMovies.size(); i++) {
            int movieId = genreMovies[i];
            if (movies[movieId].industry == industry) {
                industryGenreMovies.push_back(movieId);
            }
        }
        
        if (industryGenreMovies.empty()) return recommendations;
        
        // Calculate average similarity for each movie within the genre and industry
        vector<pair<double, int> > movieScores;
        
        for (int i = 0; i < industryGenreMovies.size(); i++) {
            int movieId = industryGenreMovies[i];
            double totalSimilarity = 0.0;
            int similarCount = 0;
            
            for (int j = 0; j < adjList[movieId].size(); j++) {
                int neighborId = adjList[movieId][j].first;
                if (movies[neighborId].genre == genre && movies[neighborId].industry == industry) {
                    totalSimilarity += adjList[movieId][j].second;
                    similarCount++;
                }
            }
            
            double avgSimilarity = (similarCount > 0) ? totalSimilarity / similarCount : 0.0;
            // Combine rating and graph similarity
            double score = movies[movieId].rating * 0.6 + avgSimilarity * 4.0;
            movieScores.push_back(make_pair(score, movieId));
        }
        
        sort(movieScores.begin(), movieScores.end(), compareByRating);
        
        // Take top N
        for (int i = 0; i < min(topN, (int)movieScores.size()); i++) {
            recommendations.push_back(movies[movieScores[i].second]);
        }
        
        return recommendations;
    }
    
    // Find similar movies to a given movie
    vector<Movie> findSimilarMovies(const string& movieTitle, int topN = 3) {
        vector<Movie> recommendations;
        
        if (titleToId.find(movieTitle) == titleToId.end()) {
            return recommendations;
        }
        
        int movieId = titleToId[movieTitle];
        
        // Get directly connected movies with highest similarity
        vector<pair<double, int> > similarMovies;
        
        for (int i = 0; i < adjList[movieId].size(); i++) {
            int neighborId = adjList[movieId][i].first;
            double similarity = adjList[movieId][i].second;
            similarMovies.push_back(make_pair(similarity, neighborId));
        }
        
        sort(similarMovies.begin(), similarMovies.end(), compareByRating);
        
        for (int i = 0; i < min(topN, (int)similarMovies.size()); i++) {
            recommendations.push_back(movies[similarMovies[i].second]);
        }
        
        return recommendations;
    }
};

// Helper function to display recommendations
void displayRecommendations(const vector<Movie>& recommendations, const string& method, int displayCount = 5) {
    cout << "\n" << method << " (Top " << displayCount << "):\n";
    if (recommendations.empty()) {
        cout << "   No recommendations found.\n";
        return;
    }
    int count = min(displayCount, (int)recommendations.size());
    for (int i = 0; i < count; i++) {
        const Movie& movie = recommendations[i];
        cout << "   " << (i+1) << ". " << movie.title 
             << " (Rating: " << movie.rating 
             << ", Actor: " << movie.actor << ")\n";
    }
}

// Function to display popular actors in a genre for a specific industry
void displayPopularActors(const string& genre, const vector<Movie>& genreMovies, int displayCount = 3) {
    // Count movies by actor in this genre
    map<string, int> actorCount;
    map<string, double> actorRatingSum;
    
    for (int i = 0; i < genreMovies.size(); i++) {
        Movie m = genreMovies[i];
        actorCount[m.actor]++;
        actorRatingSum[m.actor] += m.rating;
    }
    
    // Display top actors in this genre
    vector<pair<double, string> > topActors;
    map<string, int>::iterator it;
    
    for (it = actorCount.begin(); it != actorCount.end(); ++it) {
        string actor = it->first;
        double avgRating = actorRatingSum[actor] / it->second;
        topActors.push_back(make_pair(avgRating, actor));
    }
    
    sort(topActors.begin(), topActors.end(), compareActorByRating);
    
    cout << "\nPopular Actors in " << genre << " (Top " << displayCount << "):\n";
    int count = min(displayCount, (int)topActors.size());
    for (int i = 0; i < count; i++) {
        cout << "   " << (i+1) << ". " << topActors[i].second 
             << " (Appears in " << actorCount[topActors[i].second] 
             << " movies, Avg Rating: " << topActors[i].first << ")\n";
    }
}

// Function to clear input stream and handle invalid input
void clearInputStream() {
    cin.clear();
    cin.ignore(10000, '\n');
}

int main() {
    cout << "==========================================\n";
    cout << "   MOVIE RECOMMENDATION SYSTEM\n";
    cout << "==========================================\n";
    
    // Create sample movies
    vector<Movie> sampleMovies;
    int movieId = 1;
    
    // ============ HOLLYWOOD MOVIES ============
    
    // Hollywood Sci-Fi Movies (13 movies)
    sampleMovies.push_back(Movie(movieId++, "Inception", "Sci-Fi", "Leonardo DiCaprio", 8.8, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "Interstellar", "Sci-Fi", "Matthew McConaughey", 8.6, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "The Matrix", "Sci-Fi", "Keanu Reeves", 8.7, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "Arrival", "Sci-Fi", "Amy Adams", 7.9, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "Blade Runner 2049", "Sci-Fi", "Ryan Gosling", 8.0, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "Tenet", "Sci-Fi", "John David Washington", 7.5, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "Avatar", "Sci-Fi", "Sam Worthington", 7.8, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "Gravity", "Sci-Fi", "Sandra Bullock", 7.7, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "The Martian", "Sci-Fi", "Matt Damon", 8.0, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "Edge of Tomorrow", "Sci-Fi", "Tom Cruise", 7.9, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "Looper", "Sci-Fi", "Joseph Gordon-Levitt", 7.4, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "Moon", "Sci-Fi", "Sam Rockwell", 7.8, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "District 9", "Sci-Fi", "Sharlto Copley", 7.9, "Hollywood"));
    
    // Hollywood Action Movies (13 movies)
    sampleMovies.push_back(Movie(movieId++, "The Dark Knight", "Action", "Christian Bale", 9.0, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "Batman Begins", "Action", "Christian Bale", 8.2, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "The Dark Knight Rises", "Action", "Christian Bale", 8.4, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "Mad Max: Fury Road", "Action", "Tom Hardy", 8.1, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "John Wick", "Action", "Keanu Reeves", 7.4, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "Die Hard", "Action", "Bruce Willis", 8.2, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "Gladiator", "Action", "Russell Crowe", 8.5, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "Braveheart", "Action", "Mel Gibson", 8.3, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "The Matrix", "Action", "Keanu Reeves", 8.7, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "Terminator 2", "Action", "Arnold Schwarzenegger", 8.5, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "Die Hard 2", "Action", "Bruce Willis", 7.1, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "The Bourne Identity", "Action", "Matt Damon", 7.9, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "Mission Impossible", "Action", "Tom Cruise", 7.1, "Hollywood"));
    
    // Hollywood Drama Movies (13 movies)
    sampleMovies.push_back(Movie(movieId++, "The Prestige", "Drama", "Christian Bale", 8.5, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "Catch Me If You Can", "Drama", "Leonardo DiCaprio", 8.1, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "The Shawshank Redemption", "Drama", "Tim Robbins", 9.3, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "Forrest Gump", "Drama", "Tom Hanks", 8.8, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "The Green Mile", "Drama", "Tom Hanks", 8.6, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "The Godfather", "Drama", "Marlon Brando", 9.2, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "Schindler's List", "Drama", "Liam Neeson", 8.9, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "Fight Club", "Drama", "Brad Pitt", 8.8, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "Pulp Fiction", "Drama", "John Travolta", 8.9, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "Good Will Hunting", "Drama", "Matt Damon", 8.3, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "A Beautiful Mind", "Drama", "Russell Crowe", 8.2, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "The Social Network", "Drama", "Jesse Eisenberg", 7.7, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "The Pursuit of Happyness", "Drama", "Will Smith", 8.0, "Hollywood"));
    
    // Hollywood Thriller Movies (12 movies)
    sampleMovies.push_back(Movie(movieId++, "Shutter Island", "Thriller", "Leonardo DiCaprio", 8.2, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "Memento", "Thriller", "Guy Pearce", 8.4, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "Gone Girl", "Thriller", "Ben Affleck", 8.1, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "Se7en", "Thriller", "Brad Pitt", 8.6, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "The Silence of the Lambs", "Thriller", "Jodie Foster", 8.6, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "Psycho", "Thriller", "Anthony Perkins", 8.5, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "The Usual Suspects", "Thriller", "Kevin Spacey", 8.5, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "Fargo", "Thriller", "Frances McDormand", 8.1, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "Zodiac", "Thriller", "Jake Gyllenhaal", 7.7, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "Prisoners", "Thriller", "Hugh Jackman", 8.1, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "Nightcrawler", "Thriller", "Jake Gyllenhaal", 7.8, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "The Girl with the Dragon Tattoo", "Thriller", "Daniel Craig", 7.8, "Hollywood"));
    
    // Hollywood Comedy Movies (12 movies)
    sampleMovies.push_back(Movie(movieId++, "The Wolf of Wall Street", "Comedy", "Leonardo DiCaprio", 8.2, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "Superbad", "Comedy", "Jonah Hill", 7.6, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "The Hangover", "Comedy", "Bradley Cooper", 7.7, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "Bridesmaids", "Comedy", "Kristen Wiig", 6.8, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "Anchorman", "Comedy", "Will Ferrell", 7.2, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "Step Brothers", "Comedy", "Will Ferrell", 6.9, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "Talladega Nights", "Comedy", "Will Ferrell", 6.6, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "The 40-Year-Old Virgin", "Comedy", "Steve Carell", 7.1, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "Knocked Up", "Comedy", "Seth Rogen", 7.0, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "Forgetting Sarah Marshall", "Comedy", "Jason Segel", 7.1, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "Groundhog Day", "Comedy", "Bill Murray", 8.0, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "Mrs. Doubtfire", "Comedy", "Robin Williams", 7.1, "Hollywood"));
    
    // Hollywood War Movies (10 movies)
    sampleMovies.push_back(Movie(movieId++, "Dunkirk", "War", "Tom Hardy", 7.9, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "Saving Private Ryan", "War", "Tom Hanks", 8.6, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "1917", "War", "George MacKay", 8.3, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "Apocalypse Now", "War", "Martin Sheen", 8.4, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "Full Metal Jacket", "War", "Matthew Modine", 8.3, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "Platoon", "War", "Charlie Sheen", 8.1, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "Black Hawk Down", "War", "Josh Hartnett", 7.7, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "The Hurt Locker", "War", "Jeremy Renner", 7.5, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "American Sniper", "War", "Bradley Cooper", 7.3, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "Hacksaw Ridge", "War", "Andrew Garfield", 8.1, "Hollywood"));
    
    // Hollywood Rom-Com Movies (12 movies)
    sampleMovies.push_back(Movie(movieId++, "Crazy Rich Asians", "Rom-Com", "Constance Wu", 7.0, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "The Proposal", "Rom-Com", "Sandra Bullock", 7.3, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "10 Things I Hate About You", "Rom-Com", "Heath Ledger", 7.3, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "When Harry Met Sally", "Rom-Com", "Meg Ryan", 7.6, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "Notting Hill", "Rom-Com", "Julia Roberts", 7.2, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "Love Actually", "Rom-Com", "Hugh Grant", 7.6, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "Pretty Woman", "Rom-Com", "Julia Roberts", 7.1, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "27 Dresses", "Rom-Com", "Katherine Heigl", 6.5, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "The Holiday", "Rom-Com", "Cameron Diaz", 7.0, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "How to Lose a Guy in 10 Days", "Rom-Com", "Kate Hudson", 6.8, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "Mamma Mia!", "Rom-Com", "Meryl Streep", 6.5, "Hollywood"));
    sampleMovies.push_back(Movie(movieId++, "The Devil Wears Prada", "Rom-Com", "Anne Hathaway", 7.5, "Hollywood"));
    
    // ============ BOLLYWOOD MOVIES ============
    
    // Bollywood Action Movies (12 movies)
    sampleMovies.push_back(Movie(movieId++, "Pathaan", "Action", "Shah Rukh Khan", 7.5, "Bollywood"));
    sampleMovies.push_back(Movie(movieId++, "War", "Action", "Hrithik Roshan", 7.8, "Bollywood"));
    sampleMovies.push_back(Movie(movieId++, "Tiger Zinda Hai", "Action", "Salman Khan", 7.2, "Bollywood"));
    sampleMovies.push_back(Movie(movieId++, "Dhoom 3", "Action", "Aamir Khan", 7.1, "Bollywood"));
    sampleMovies.push_back(Movie(movieId++, "Bang Bang", "Action", "Hrithik Roshan", 7.0, "Bollywood"));
    sampleMovies.push_back(Movie(movieId++, "Singham", "Action", "Ajay Devgn", 7.3, "Bollywood"));
    sampleMovies.push_back(Movie(movieId++, "Don 2", "Action", "Shah Rukh Khan", 7.6, "Bollywood"));
    sampleMovies.push_back(Movie(movieId++, "Krrish 3", "Action", "Hrithik Roshan", 6.9, "Bollywood"));
    sampleMovies.push_back(Movie(movieId++, "Chennai Express", "Action", "Shah Rukh Khan", 7.2, "Bollywood"));
    sampleMovies.push_back(Movie(movieId++, "Dabangg", "Action", "Salman Khan", 7.1, "Bollywood"));
    sampleMovies.push_back(Movie(movieId++, "Race 3", "Action", "Salman Khan", 5.2, "Bollywood"));
    sampleMovies.push_back(Movie(movieId++, "Simmba", "Action", "Ranveer Singh", 7.0, "Bollywood"));
    
    // Bollywood Drama Movies (12 movies)
    sampleMovies.push_back(Movie(movieId++, "3 Idiots", "Drama", "Aamir Khan", 9.1, "Bollywood"));
    sampleMovies.push_back(Movie(movieId++, "Dangal", "Drama", "Aamir Khan", 9.0, "Bollywood"));
    sampleMovies.push_back(Movie(movieId++, "Taare Zameen Par", "Drama", "Aamir Khan", 8.8, "Bollywood"));
    sampleMovies.push_back(Movie(movieId++, "Bajrangi Bhaijaan", "Drama", "Salman Khan", 8.5, "Bollywood"));
    sampleMovies.push_back(Movie(movieId++, "PK", "Drama", "Aamir Khan", 8.6, "Bollywood"));
    sampleMovies.push_back(Movie(movieId++, "Swades", "Drama", "Shah Rukh Khan", 8.7, "Bollywood"));
    sampleMovies.push_back(Movie(movieId++, "Chak De India", "Drama", "Shah Rukh Khan", 8.5, "Bollywood"));
    sampleMovies.push_back(Movie(movieId++, "Queen", "Drama", "Kangana Ranaut", 8.6, "Bollywood"));
    sampleMovies.push_back(Movie(movieId++, "Udaan", "Drama", "Rajat Barmecha", 8.5, "Bollywood"));
    sampleMovies.push_back(Movie(movieId++, "Zindagi Na Milegi Dobara", "Drama", "Hrithik Roshan", 8.4, "Bollywood"));
    sampleMovies.push_back(Movie(movieId++, "Barfi!", "Drama", "Ranbir Kapoor", 8.3, "Bollywood"));
    sampleMovies.push_back(Movie(movieId++, "Yeh Jawaani Hai Deewani", "Drama", "Ranbir Kapoor", 7.9, "Bollywood"));
    
    // Bollywood Comedy Movies (12 movies)
    sampleMovies.push_back(Movie(movieId++, "Hera Pheri", "Comedy", "Akshay Kumar", 8.5, "Bollywood"));
    sampleMovies.push_back(Movie(movieId++, "Phir Hera Pheri", "Comedy", "Akshay Kumar", 8.1, "Bollywood"));
    sampleMovies.push_back(Movie(movieId++, "Golmaal", "Comedy", "Ajay Devgn", 7.8, "Bollywood"));
    sampleMovies.push_back(Movie(movieId++, "Golmaal Returns", "Comedy", "Ajay Devgn", 7.3, "Bollywood"));
    sampleMovies.push_back(Movie(movieId++, "Dhamaal", "Comedy", "Arshad Warsi", 7.6, "Bollywood"));
    sampleMovies.push_back(Movie(movieId++, "Welcome", "Comedy", "Akshay Kumar", 7.5, "Bollywood"));
    sampleMovies.push_back(Movie(movieId++, "Bhool Bhulaiyaa", "Comedy", "Akshay Kumar", 7.9, "Bollywood"));
    sampleMovies.push_back(Movie(movieId++, "Hungama", "Comedy", "Akshaye Khanna", 7.8, "Bollywood"));
    sampleMovies.push_back(Movie(movieId++, "Andaz Apna Apna", "Comedy", "Aamir Khan", 8.2, "Bollywood"));
    sampleMovies.push_back(Movie(movieId++, "Chup Chup Ke", "Comedy", "Shahid Kapoor", 7.4, "Bollywood"));
    sampleMovies.push_back(Movie(movieId++, "De Dana Dan", "Comedy", "Akshay Kumar", 7.2, "Bollywood"));
    sampleMovies.push_back(Movie(movieId++, "Housefull", "Comedy", "Akshay Kumar", 6.8, "Bollywood"));
    
    // Bollywood Thriller Movies (12 movies)
    sampleMovies.push_back(Movie(movieId++, "Drishyam", "Thriller", "Ajay Devgn", 8.6, "Bollywood"));
    sampleMovies.push_back(Movie(movieId++, "Kahaani", "Thriller", "Vidya Balan", 8.4, "Bollywood"));
    sampleMovies.push_back(Movie(movieId++, "Andhadhun", "Thriller", "Ayushmann Khurrana", 8.7, "Bollywood"));
    sampleMovies.push_back(Movie(movieId++, "Talaash", "Thriller", "Aamir Khan", 7.9, "Bollywood"));
    sampleMovies.push_back(Movie(movieId++, "Badla", "Thriller", "Amitabh Bachchan", 8.2, "Bollywood"));
    sampleMovies.push_back(Movie(movieId++, "Raat Akeli Hai", "Thriller", "Nawazuddin Siddiqui", 8.0, "Bollywood"));
    sampleMovies.push_back(Movie(movieId++, "Ugly", "Thriller", "Rahul Bhat", 8.1, "Bollywood"));
    sampleMovies.push_back(Movie(movieId++, "A Wednesday", "Thriller", "Naseeruddin Shah", 8.5, "Bollywood"));
    sampleMovies.push_back(Movie(movieId++, "Kahani 2", "Thriller", "Vidya Balan", 7.1, "Bollywood"));
    sampleMovies.push_back(Movie(movieId++, "Ittefaq", "Thriller", "Sidharth Malhotra", 7.4, "Bollywood"));
    sampleMovies.push_back(Movie(movieId++, "Raman Raghav 2.0", "Thriller", "Nawazuddin Siddiqui", 7.7, "Bollywood"));
    sampleMovies.push_back(Movie(movieId++, "Jersey", "Thriller", "Shahid Kapoor", 8.3, "Bollywood"));
    
    // Bollywood Rom-Com Movies (12 movies)
    sampleMovies.push_back(Movie(movieId++, "Jab We Met", "Rom-Com", "Shahid Kapoor", 8.3, "Bollywood"));
    sampleMovies.push_back(Movie(movieId++, "Yeh Jawaani Hai Deewani", "Rom-Com", "Ranbir Kapoor", 7.9, "Bollywood"));
    sampleMovies.push_back(Movie(movieId++, "2 States", "Rom-Com", "Arjun Kapoor", 7.5, "Bollywood"));
    sampleMovies.push_back(Movie(movieId++, "Ek Main Aur Ekk Tu", "Rom-Com", "Imran Khan", 6.9, "Bollywood"));
    sampleMovies.push_back(Movie(movieId++, "Badrinath Ki Dulhania", "Rom-Com", "Varun Dhawan", 7.2, "Bollywood"));
    sampleMovies.push_back(Movie(movieId++, "Humpty Sharma Ki Dulhania", "Rom-Com", "Varun Dhawan", 7.0, "Bollywood"));
    sampleMovies.push_back(Movie(movieId++, "Dilwale Dulhania Le Jayenge", "Rom-Com", "Shah Rukh Khan", 8.5, "Bollywood"));
    sampleMovies.push_back(Movie(movieId++, "Kuch Kuch Hota Hai", "Rom-Com", "Shah Rukh Khan", 8.2, "Bollywood"));
    sampleMovies.push_back(Movie(movieId++, "Kal Ho Naa Ho", "Rom-Com", "Shah Rukh Khan", 8.1, "Bollywood"));
    sampleMovies.push_back(Movie(movieId++, "Kabhi Khushi Kabhie Gham", "Rom-Com", "Shah Rukh Khan", 7.8, "Bollywood"));
    sampleMovies.push_back(Movie(movieId++, "Ae Dil Hai Mushkil", "Rom-Com", "Ranbir Kapoor", 7.4, "Bollywood"));
    sampleMovies.push_back(Movie(movieId++, "Tamasha", "Rom-Com", "Ranbir Kapoor", 7.8, "Bollywood"));
    
    // Initialize Content-Based Recommender
    ContentBasedRecommender contentRecommender;
    for (int i = 0; i < sampleMovies.size(); i++) {
        contentRecommender.addMovie(sampleMovies[i]);
    }
    
    // Initialize Graph-Based Recommender
    GraphBasedRecommender graphRecommender;
    for (int i = 0; i < sampleMovies.size(); i++) {
        graphRecommender.addMovie(sampleMovies[i]);
    }
    
    // Build similarity graph
    graphRecommender.buildSimilarityGraph();
    
    cout << "\nWELCOME TO MOVIE RECOMMENDATION SYSTEM\n";
    cout << "=============================================\n";
    cout << "Total Movies in Database: " << sampleMovies.size() << "\n";
    cout << "=============================================\n";
    
    while (true) {
        // First ask for industry preference
        cout << "\nSelect Industry:\n";
        cout << "   1. Hollywood\n";
        cout << "   2. Bollywood\n";
        cout << "   3. Exit\n";
        cout << "\nEnter your choice (1-3): ";
        
        int industryChoice;
        cin >> industryChoice;
        
        if (cin.fail()) {
            clearInputStream();
            cout << "Invalid input! Please enter a number.\n";
            continue;
        }
        
        if (industryChoice == 3) {
            cout << "\nThank you for using the Movie Recommendation System!\n";
            break;
        }
        
        if (industryChoice < 1 || industryChoice > 2) {
            cout << "Invalid choice! Please try again.\n";
            continue;
        }
        
        string selectedIndustry = (industryChoice == 1) ? "Hollywood" : "Bollywood";
        
        cout << "\nSELECTED INDUSTRY: " << selectedIndustry << "\n";
        cout << "=============================================\n";
        
        // Get genres available in this industry
        vector<string> industryGenres = contentRecommender.getGenresByIndustry(selectedIndustry);
        
        if (industryGenres.empty()) {
            cout << "No genres found for " << selectedIndustry << "\n";
            continue;
        }
        
        // Show genre selection menu
        cout << "\nAvailable Genres in " << selectedIndustry << ":\n";
        for (int i = 0; i < industryGenres.size(); i++) {
            cout << "   " << (i+1) << ". " << industryGenres[i] << endl;
        }
        cout << "   " << (industryGenres.size() + 1) << ". Back to Industry Selection\n";
        
        cout << "\nSelect a genre (enter number): ";
        int genreChoice;
        cin >> genreChoice;
        
        if (cin.fail()) {
            clearInputStream();
            cout << "Invalid input! Please enter a number.\n";
            continue;
        }
        
        if (genreChoice == industryGenres.size() + 1) {
            continue; // Go back to industry selection
        }
        
        if (genreChoice < 1 || genreChoice > industryGenres.size()) {
            cout << "Invalid choice! Please try again.\n";
            continue;
        }
        
        string selectedGenre = industryGenres[genreChoice - 1];
        
        cout << "\nSELECTED GENRE: " << selectedGenre << " (" << selectedIndustry << ")\n";
        cout << "=============================================\n";
        
        // Get movies in this genre and industry
        vector<pair<Movie, int> > genreMoviesWithIndices = 
            contentRecommender.getMoviesInGenreWithIndices(selectedGenre, selectedIndustry);
        vector<Movie> genreMovies = 
            contentRecommender.getMoviesByGenreAndIndustry(selectedGenre, selectedIndustry);
        
        cout << "\nTotal " << selectedGenre << " movies in " << selectedIndustry << ": " << genreMovies.size() << "\n";
        
        // Ask for recommendation type
        cout << "\nWhat would you like to see?\n";
        cout << "   1. Top Rated in Genre (Top 5)\n";
        cout << "   2. Graph-Based (Similarity) Recommendations (Top 5)\n";
        cout << "   3. Popular Actors in this Genre (Top 3)\n";
        cout << "   4. All Recommendations (show everything)\n";
        cout << "\nEnter your choice (1-4): ";
        
        int recChoice;
        cin >> recChoice;
        
        if (cin.fail()) {
            clearInputStream();
            cout << "Invalid input! Showing default (Top Rated).\n";
            recChoice = 1;
        }
        
        cout << "\n";
        
        // Store the recommendations that will be shown
        vector<Movie> displayedRecommendations;
        
        switch(recChoice) {
            case 1: {
                // Top Rated in Genre only
                displayedRecommendations = contentRecommender.recommendByGenreAndIndustry(selectedGenre, selectedIndustry, 5);
                displayRecommendations(displayedRecommendations, "Top Rated in Genre", 5);
                break;
            }
            case 2: {
                // Graph-Based Similarity only
                displayedRecommendations = graphRecommender.recommendByGenreGraph(selectedGenre, selectedIndustry, 5);
                displayRecommendations(displayedRecommendations, "Graph-Based (Similarity) Recommendations", 5);
                break;
            }
            case 3: {
                // Popular Actors only
                displayPopularActors(selectedGenre, genreMovies, 3);
                cout << "\nPress Enter to continue...";
                cin.ignore(10000, '\n');
                cin.get();
                continue;
            }
            case 4: {
                // Show all recommendations
                cout << "\nALL RECOMMENDATIONS FOR " << selectedGenre << " (" << selectedIndustry << "):\n";
                cout << "----------------------------------------\n";
                
                vector<Movie> topRatedRecs = contentRecommender.recommendByGenreAndIndustry(selectedGenre, selectedIndustry, 5);
                displayRecommendations(topRatedRecs, "Top Rated in Genre", 5);
                
                vector<Movie> graphRecs = graphRecommender.recommendByGenreGraph(selectedGenre, selectedIndustry, 5);
                displayRecommendations(graphRecs, "Graph-Based (Similarity)", 5);
                
                displayPopularActors(selectedGenre, genreMovies, 3);
                
                displayedRecommendations = topRatedRecs;
                break;
            }
            default: {
                cout << "Invalid choice! Showing default (Top Rated).\n";
                displayedRecommendations = contentRecommender.recommendByGenreAndIndustry(selectedGenre, selectedIndustry, 5);
                displayRecommendations(displayedRecommendations, "Top Rated in Genre", 5);
            }
        }
        
        // Ask for movie details
        if (!displayedRecommendations.empty() && recChoice != 3) {
            cout << "\nWould you like to see details of any movie from the recommendations?\n";
            cout << "Enter the movie number (1-" << displayedRecommendations.size() << ") to see details,\n";
            cout << "or enter 0 to continue to genre selection: ";
            
            int movieChoice;
            cin >> movieChoice;
            
            if (cin.fail()) {
                clearInputStream();
                cout << "Invalid input! Returning to genre selection.\n";
                continue;
            }
            
            if (movieChoice > 0 && movieChoice <= displayedRecommendations.size()) {
                Movie selectedMovie = displayedRecommendations[movieChoice - 1];
                
                cout << "\n" << string(40, '=') << "\n";
                cout << "MOVIE DETAILS:\n";
                cout << string(40, '=') << "\n";
                cout << "   Title: " << selectedMovie.title << endl;
                cout << "   Genre: " << selectedMovie.genre << endl;
                cout << "   Actor: " << selectedMovie.actor << endl;
                cout << "   Rating: " << selectedMovie.rating << "/10\n";
                cout << "   Industry: " << selectedMovie.industry << endl;
                
                // Find similar movies
                cout << "\nIf you like " << selectedMovie.title << ", you might also like (Top 3):\n";
                
                vector<Movie> similarMovies = graphRecommender.findSimilarMovies(selectedMovie.title, 3);
                if (!similarMovies.empty()) {
                    for (int i = 0; i < similarMovies.size(); i++) {
                        cout << "   * " << similarMovies[i].title 
                             << " (Rating: " << similarMovies[i].rating << ")\n";
                    }
                }
                
                // Get actor-based recommendations
                vector<Movie> actorRecs = contentRecommender.recommendByActor(selectedMovie.actor, 2);
                bool hasActorRecs = false;
                for (int i = 0; i < actorRecs.size(); i++) {
                    if (actorRecs[i].title != selectedMovie.title) {
                        bool alreadyShown = false;
                        for (int j = 0; j < similarMovies.size(); j++) {
                            if (similarMovies[j].title == actorRecs[i].title) {
                                alreadyShown = true;
                                break;
                            }
                        }
                        if (!alreadyShown) {
                            if (!hasActorRecs) {
                                cout << "\n   Other movies with " << selectedMovie.actor << " (Top 2):\n";
                                hasActorRecs = true;
                            }
                            cout << "   * " << actorRecs[i].title 
                                 << " (Rating: " << actorRecs[i].rating << ")\n";
                        }
                    }
                }
                
                cout << "\n" << string(40, '-') << "\n";
                cout << "\nPress Enter to continue...";
                cin.ignore(10000, '\n');
                cin.get();
            }
            else if (movieChoice == 0) {
                continue;
            }
            else {
                cout << "Invalid movie number! Returning to genre selection.\n";
            }
        }
    }
    
    return 0;
}
