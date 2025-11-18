#ifndef JIKAN_H
#define JIKAN_H

#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <pplx/pplx.h>
#include <iostream>
#include <string>
#include <vector>

using namespace web;
using namespace web::http;
using namespace web::http::client;

class Jikan {
private:
    std::string api_base = "https://api.jikan.moe/v4";
    http_client_config client_config;
    
    http_request create_request(const std::string& endpoint, const std::string& method, const std::string& data = "") {
        http_request request;
        
        if (method == "GET") {
            request.set_method(methods::GET);
        } else if (method == "POST") {
            request.set_method(methods::POST);
        } else if (method == "PUT") {
            request.set_method(methods::PUT);
        } else if (method == "DEL") {
            request.set_method(methods::DEL);
        }
        
        request.set_request_uri(utility::conversions::to_string_t(endpoint));
        
        // Set headers
        request.headers().add(U("Host"), U("api.jikan.moe"));
        request.headers().add(U("User-Agent"), U("Mozilla/5.0 (X11; Linux x86_64; rv:109.0) Gecko/20100101 Firefox/115.0"));
        
        if (!data.empty() && (method == "POST" || method == "PUT")) {
            request.set_body(data);
        }
        
        return request;
    }
    
    std::string build_query_params(const std::map<std::string, std::string>& params) {
        if (params.empty()) return "";
        
        std::string query = "?";
        bool first = true;
        for (const auto& param : params) {
            if (!param.second.empty()) {
                if (!first) query += "&";
                // Кодируем значения параметров
                auto encoded_value = web::uri::encode_data_string(utility::conversions::to_string_t(param.second));
                query += param.first + "=" + utility::conversions::to_utf8string(encoded_value);
                first = false;
            }
        }
        return query;
    }
    
    pplx::task<json::value> make_api_call(const std::string& endpoint, const std::string& method, const std::string& data = "") {
        http_client client(utility::conversions::to_string_t(api_base), client_config);
        auto request = create_request(endpoint, method, data);

        return client.request(request)
            .then([](http_response response) {
                if (response.status_code() == status_codes::OK) {
                    return response.extract_json();
                } else {
                    json::value error_obj;
                    error_obj[U("error")] = json::value::string(
                        U("HTTP Error: ") + utility::conversions::to_string_t(std::to_string(response.status_code())));
                    error_obj[U("success")] = json::value::boolean(false);
                    return pplx::task_from_result(error_obj);
                }
            })
            .then([](pplx::task<json::value> previousTask) {
                try {
                    return previousTask.get();
                } catch (const std::exception& e) {
                    json::value error_obj;
                    error_obj[U("error")] = json::value::string(
                        U("Exception: ") + utility::conversions::to_string_t(e.what()));
                    error_obj[U("success")] = json::value::boolean(false);
                    return error_obj;
                }
            });
    }

public:
    Jikan(){
        client_config.set_validate_certificates(false);
    }

    pplx::task<json::value> getAnimeById(int id) {
        return make_api_call("/anime/" + std::to_string(id),"GET");
    }

    pplx::task<json::value> getAnimeCharacters(int id) {
        return make_api_call("/anime/" + std::to_string(id) + "/characters","GET");
    }
    pplx::task<json::value> getAnimeStaff(int id) {
        return make_api_call("/anime/" + std::to_string(id) + "/staff","GET");
    }

    pplx::task<json::value> getAnimeEpisodes(int id) {
        return make_api_call("/anime/" + std::to_string(id) + "/episodes","GET");
    }
    pplx::task<json::value> getAnimeEpisodeById(int id,int episode) {
        return make_api_call("/anime/" + std::to_string(id) + "/episodes/" + std::to_string(episode),"GET");
    }
    pplx::task<json::value> getAnimeNews(int id) {
        return make_api_call("/anime/" + std::to_string(id) + "/news","GET");
    }
    pplx::task<json::value> getAnimeForum(int id) {
        return make_api_call("/anime/" + std::to_string(id) + "/forum","GET");
    }
    pplx::task<json::value> getAnimeVideos(int id) {
        return make_api_call("/anime/" + std::to_string(id) + "/videos","GET");
    }
    pplx::task<json::value> getAnimeVideosEpisodes(int id) {
        return make_api_call("/anime/" + std::to_string(id) + "/videos/episodes","GET");
    }
    pplx::task<json::value> getAnimePictures(int id) {
        return make_api_call("/anime/" + std::to_string(id) + "/pictures","GET");
    }
    pplx::task<json::value> getAnimeStatistics(int id) {
        return make_api_call("/anime/" + std::to_string(id) + "/statistics","GET");
    }
    pplx::task<json::value> getAnimeMoreInfo(int id) {
        return make_api_call("/anime/" + std::to_string(id) + "/moreinfo","GET");
    }
    pplx::task<json::value> getAnimeRecommendations(int id) {
        return make_api_call("/anime/" + std::to_string(id) + "/recommendations","GET");
    }
    pplx::task<json::value> getAnimeUserUpdates(int id) {
        return make_api_call("/anime/" + std::to_string(id) + "/userupdates","GET");
    }
    pplx::task<json::value> getAnimeReviews(int id) {
        return make_api_call("/anime/" + std::to_string(id) + "/reviews","GET");
    }
    pplx::task<json::value> getAnimeRelations(int id) {
        return make_api_call("/anime/" + std::to_string(id) + "/relations","GET");
    }
    pplx::task<json::value> getAnimeThemes(int id) {
        return make_api_call("/anime/" + std::to_string(id) + "/themes","GET");
    }
    pplx::task<json::value> getAnimeExternal(int id) {
        return make_api_call("/anime/" + std::to_string(id) + "/external","GET");
    }
    pplx::task<json::value> getAnimeStreaming(int id) {
        return make_api_call("/anime/" + std::to_string(id) + "/streaming","GET");
    }
    pplx::task<json::value> getAnimeFullById(int id) {
        return make_api_call("/anime/" + std::to_string(id) + "/full","GET");
    }
    pplx::task<json::value> getAnimeSearch(int page = 0,int limit = 0,const std::string& q = "", const std::string& type = "",double score = 0.0,double min_score = 0.0,double max_score = 0.0,const std::string& status = "", const std::string& rating = "",bool sfw = false,const std::string& genres = "", const std::string& genres_exclude = "", const std::string& order_by = "", const std::string& sort = "", const std::string& letter = "", const std::string& producers = "", const std::string& start_date = "", const std::string& end_date = "",bool unapproved = false) {
        std::map<std::string, std::string> params;
        if (unapproved) params["unapproved"] = "";
        if (page > 0) params["page"] = std::to_string(page);
        if (limit > 0) params["limit"] = std::to_string(limit);
        if (!q.empty()) params["q"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(q)));
        if (!type.empty()) params["type"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(type)));
        if (score > 0) params["score"] = std::to_string(score);
        if (min_score > 0) params["min_score"] = std::to_string(min_score);
        if (max_score > 0) params["max_score"] = std::to_string(max_score);
        if (!status.empty()) params["status"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(status)));
        if (!rating.empty()) params["rating"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(rating)));
        if (sfw) params["sfw"] = "";
        if (!genres.empty()) params["genres"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(genres)));
        if (!genres_exclude.empty()) params["genres_exclude"] = genres_exclude;
        if (!order_by.empty()) params["order_by"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(order_by)));
        if (!sort.empty()) params["sort"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(sort)));
        if (!letter.empty()) params["letter"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(letter)));
        if (!producers.empty()) params["producers"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(producers)));
        if (!start_date.empty()) params["start_date"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(start_date)));
        if (!end_date.empty()) params["end_date"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(end_date)));
        std::string url = "/anime" + build_query_params(params);
        return make_api_call(url, "GET");
    }

    pplx::task<json::value> getCharacterFullById(int id) {
        return make_api_call("/characters/" + std::to_string(id) + "/full","GET");
    }
    pplx::task<json::value> getCharacterById(int id) {
        return make_api_call("/characters/" + std::to_string(id),"GET");
    }
    pplx::task<json::value> getCharacterAnime(int id) {
        return make_api_call("/characters/" + std::to_string(id) + "/anime","GET");
    }
    pplx::task<json::value> getCharacterManga(int id) {
        return make_api_call("/characters/" + std::to_string(id) + "/manga","GET");
    }
    pplx::task<json::value> getCharacterVoiceActors(int id) {
        return make_api_call("/characters/" + std::to_string(id) + "/voices","GET");
    }
    pplx::task<json::value> getCharacterPictures(int id) {
        return make_api_call("/characters/" + std::to_string(id) + "/pictures","GET");
    }
    pplx::task<json::value> getCharactersSearch(int page = 0,int limit = 0,const std::string& q = "", const std::string& order_by = "", const std::string& sort = "", const std::string& letter = "") {
        std::map<std::string, std::string> params;
        if (page > 0) params["page"] = std::to_string(page);
        if (limit > 0) params["limit"] = std::to_string(limit);
        if (!q.empty()) params["q"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(q)));
        if (!order_by.empty()) params["order_by"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(order_by)));
        if (!sort.empty()) params["sort"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(sort)));
        if (!letter.empty()) params["letter"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(letter)));
        std::string url = "/characters" + build_query_params(params);
        return make_api_call(url, "GET");
    }

    pplx::task<json::value> getClubsById(int id) {
        return make_api_call("/clubs/" + std::to_string(id),"GET");
    }
    pplx::task<json::value> getClubMembers(int id) {
        return make_api_call("/clubs/" + std::to_string(id) + "/members","GET");
    }
    pplx::task<json::value> getClubStaff(int id) {
        return make_api_call("/clubs/" + std::to_string(id) + "/staff","GET");
    }
    pplx::task<json::value> getClubRelations(int id) {
        return make_api_call("/clubs/" + std::to_string(id) + "/relations","GET");
    }
    pplx::task<json::value> getCharactersSearch(int page = 0,int limit = 0,const std::string& q = "",const std::string& type = "", const std::string& category = "", const std::string& order_by = "", const std::string& sort = "", const std::string& letter = "") {
        std::map<std::string, std::string> params;
        if (page > 0) params["page"] = std::to_string(page);
        if (limit > 0) params["limit"] = std::to_string(limit);
        if (!q.empty()) params["q"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(q)));
        if (!type.empty()) params["type"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(type)));
        if (!category.empty()) params["category"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(category)));
        if (!order_by.empty()) params["order_by"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(order_by)));
        if (!sort.empty()) params["sort"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(sort)));
        if (!letter.empty()) params["letter"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(letter)));
        std::string url = "/clubs" + build_query_params(params);
        return make_api_call(url, "GET");
    }
    pplx::task<json::value> getAnimeGenres(const std::string& filter="") {
        std::map<std::string, std::string> params;
        if (!filter.empty()) params["filter"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(filter)));
        return make_api_call("/genres/anime" + build_query_params(params),"GET");
    }
    pplx::task<json::value> getMangaGenres(const std::string& filter="") {
        std::map<std::string, std::string> params;
        if (!filter.empty()) params["filter"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(filter)));
        return make_api_call("/genres/manga" + build_query_params(params),"GET");
    }
    pplx::task<json::value> getMagazines(int page = 0,int limit = 0,const std::string& q = "", const std::string& order_by = "", const std::string& sort = "", const std::string& letter = "") {
        std::map<std::string, std::string> params;
        if (page > 0) params["page"] = std::to_string(page);
        if (limit > 0) params["limit"] = std::to_string(limit);
        if (!q.empty()) params["q"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(q)));
        if (!order_by.empty()) params["order_by"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(order_by)));
        if (!sort.empty()) params["sort"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(sort)));
        if (!letter.empty()) params["letter"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(letter)));
        std::string url = "/magazines" + build_query_params(params);
        return make_api_call(url, "GET");
    }

    pplx::task<json::value> getMangaFullById(int id) {
        return make_api_call("/manga/" + std::to_string(id) + "/full","GET");
    }

    pplx::task<json::value> getMangaById(int id) {
        return make_api_call("/manga/" + std::to_string(id),"GET");
    }

    pplx::task<json::value> getMangaCharacters(int id) {
        return make_api_call("/manga/" + std::to_string(id) + "/characters","GET");
    }
    pplx::task<json::value> getMangaNews(int id) {
        return make_api_call("/manga/" + std::to_string(id) + "/news","GET");
    }
    pplx::task<json::value> getMangaTopics(int id) {
        return make_api_call("/mannga/" + std::to_string(id) + "/forum","GET");
    }
    pplx::task<json::value> getMangaPictures(int id) {
        return make_api_call("/manga/" + std::to_string(id) + "/pictures","GET");
    }
    pplx::task<json::value> getMangaStatistics(int id) {
        return make_api_call("/manga/" + std::to_string(id) + "/statistics","GET");
    }
    pplx::task<json::value> getMangaMoreInfo(int id) {
        return make_api_call("/manga/" + std::to_string(id) + "/moreinfo","GET");
    }
    pplx::task<json::value> getMangaRecommendations(int id) {
        return make_api_call("/manga/" + std::to_string(id) + "/recommendations","GET");
    }
    pplx::task<json::value> getMangaUserUpdates(int id) {
        return make_api_call("/manga/" + std::to_string(id) + "/userupdates","GET");
    }
    pplx::task<json::value> getMangaReviews(int id) {
        return make_api_call("/manga/" + std::to_string(id) + "/reviews","GET");
    }
    pplx::task<json::value> getMangaRelations(int id) {
        return make_api_call("/manga/" + std::to_string(id) + "/relations","GET");
    }
    pplx::task<json::value> getMangaExternal(int id) {
        return make_api_call("/manga/" + std::to_string(id) + "/external","GET");
    }
    pplx::task<json::value> getMangaSearch(int page = 0,int limit = 0,const std::string& q = "", const std::string& type = "",double score = 0.0,double min_score = 0.0,double max_score = 0.0,const std::string& status = "",bool sfw = false,const std::string& genres = "", const std::string& genres_exclude = "", const std::string& order_by = "", const std::string& sort = "", const std::string& letter = "", const std::string& magazines = "", const std::string& start_date = "", const std::string& end_date = "",bool unapproved = false) {
        std::map<std::string, std::string> params;
        if (unapproved) params["unapproved"] = "";
        if (page > 0) params["page"] = std::to_string(page);
        if (limit > 0) params["limit"] = std::to_string(limit);
        if (!q.empty()) params["q"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(q)));
        if (!type.empty()) params["type"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(type)));
        if (score > 0) params["score"] = std::to_string(score);
        if (min_score > 0) params["min_score"] = std::to_string(min_score);
        if (max_score > 0) params["max_score"] = std::to_string(max_score);
        if (!status.empty()) params["status"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(status)));
        if (sfw) params["sfw"] = "";
        if (!genres.empty()) params["genres"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(genres)));
        if (!genres_exclude.empty()) params["genres_exclude"] = genres_exclude;
        if (!order_by.empty()) params["order_by"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(order_by)));
        if (!sort.empty()) params["sort"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(sort)));
        if (!letter.empty()) params["letter"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(letter)));
        if (!magazines.empty()) params["magazines"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(magazines)));
        if (!start_date.empty()) params["start_date"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(start_date)));
        if (!end_date.empty()) params["end_date"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(end_date)));
        std::string url = "/manga" + build_query_params(params);
        return make_api_call(url, "GET");
    }
    pplx::task<json::value> getPeopleSearch(int page = 0,int limit = 0,const std::string& q = "", const std::string& order_by = "", const std::string& sort = "", const std::string& letter = "") {
        std::map<std::string, std::string> params;
        if (page > 0) params["page"] = std::to_string(page);
        if (limit > 0) params["limit"] = std::to_string(limit);
        if (!q.empty()) params["q"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(q)));
        if (!order_by.empty()) params["order_by"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(order_by)));
        if (!sort.empty()) params["sort"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(sort)));
        if (!letter.empty()) params["letter"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(letter)));
        std::string url = "/people" + build_query_params(params);
        return make_api_call(url, "GET");
    }
    pplx::task<json::value> getPersonFullById(int id) {
        return make_api_call("/people/" + std::to_string(id) + "/full","GET");
    }
    pplx::task<json::value> getPersonById(int id) {
        return make_api_call("/people/" + std::to_string(id),"GET");
    }
    pplx::task<json::value> getPersonAnime(int id) {
        return make_api_call("/people/" + std::to_string(id) + "/anime","GET");
    }
    pplx::task<json::value> getPersonManga(int id) {
        return make_api_call("/people/" + std::to_string(id) + "/manga","GET");
    }
    pplx::task<json::value> getPersonVoices(int id) {
        return make_api_call("/people/" + std::to_string(id) + "/voices","GET");
    }
    pplx::task<json::value> getPersonPictures(int id) {
        return make_api_call("/people/" + std::to_string(id) + "/pictures","GET");
    }
    pplx::task<json::value> getProducers(int page = 0,int limit = 0,const std::string& q = "", const std::string& order_by = "", const std::string& sort = "", const std::string& letter = "") {
        std::map<std::string, std::string> params;
        if (page > 0) params["page"] = std::to_string(page);
        if (limit > 0) params["limit"] = std::to_string(limit);
        if (!q.empty()) params["q"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(q)));
        if (!order_by.empty()) params["order_by"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(order_by)));
        if (!sort.empty()) params["sort"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(sort)));
        if (!letter.empty()) params["letter"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(letter)));
        std::string url = "/producers" + build_query_params(params);
        return make_api_call(url, "GET");
    }
    pplx::task<json::value> getProducerFullById(int id) {
        return make_api_call("/producers/" + std::to_string(id) + "/full","GET");
    }
    pplx::task<json::value> getProducerById(int id) {
        return make_api_call("/producers/" + std::to_string(id),"GET");
    }
    pplx::task<json::value> getProducerExternal(int id) {
        return make_api_call("/producers/" + std::to_string(id) + "/external","GET");
    }
    pplx::task<json::value> getRandomAnime() {
        return make_api_call("/random/anime","GET");
    }
    pplx::task<json::value> getRandomManga() {
        return make_api_call("/random/manga","GET");
    }
    pplx::task<json::value> getRandomCharacters() {
        return make_api_call("/random/characters","GET");
    }
    pplx::task<json::value> getRandomPeople() {
        return make_api_call("/random/people","GET");
    }
    pplx::task<json::value> getRandomUsers() {
        return make_api_call("/random/users","GET");
    }

    pplx::task<json::value> getRecentAnimeRecommendations(int page=1) {
        return make_api_call("/recommendations/anime?page=" + std::to_string(page),"GET");
    }
    pplx::task<json::value> getRecentMangaRecommendations(int page=1) {
        return make_api_call("/recommendations/manga?page=" + std::to_string(page),"GET");
    }
    pplx::task<json::value> getRecentAnimeReviews(int page=1,bool preliminary=true,bool spoilers=true) {
        return make_api_call("/reviews/anime?page=" + std::to_string(page) + "&preliminary=" + preliminary + "&spoilers=" + spoilers,"GET");
    }
    pplx::task<json::value> getRecentMangaReviews(int page) {
        return make_api_call("/reviews/manga?page=" + std::to_string(page) + "&preliminary=" + preliminary + "&spoilers=" + spoilers,"GET");
    }
    pplx::task<json::value> getSchedules(int page = 0,int limit = 0,const std::string& filter = "", bool sfw=false,bool kids=false,bool unapproved=false) {
        std::map<std::string, std::string> params;
        if (page > 0) params["page"] = std::to_string(page);
        if (limit > 0) params["limit"] = std::to_string(limit);
        if (!filter.empty()) params["filter"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(filter)));
        if (kids) params["kids"] = "true";
        if (sfw) params["sfw"] = "";
        if (unapproved) params["unapproved"] = "";
        std::string url = "/schedules" + build_query_params(params);
        return make_api_call(url, "GET");
    }
    pplx::task<json::value> getUsersSearch(int page = 0,int limit = 0,const std::string& q = "", const std::string& gender = "", const std::string& location = "",int maxAge=0,int minAge=0) {
        std::map<std::string, std::string> params;
        if (page > 0) params["page"] = std::to_string(page);
        if (limit > 0) params["limit"] = std::to_string(limit);
        if (!q.empty()) params["q"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(q)));
        if (!gender.empty()) params["gender"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(gender)));
        if (!location.empty()) params["location"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(location)));
        if (maxAge > 0) params["maxAge"] = std::to_string(maxAge);
        if (minAge > 0) params["minAge"] = std::to_string(minAge);
        std::string url = "/users" + build_query_params(params);
        return make_api_call(url, "GET");
    }
    pplx::task<json::value> getUserById(int id) {
        return make_api_call("/users/userbyid/" + std::to_string(id),"GET");
    }
    pplx::task<json::value> getUserFullProfile(const std::string& username) {
        return make_api_call("/users/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(username))) + "/full","GET");
    }
    pplx::task<json::value> getUserProfile(const std::string& username) {
        return make_api_call("/users/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(username))),"GET");
    }
    pplx::task<json::value> getUserStatistics(const std::string& username) {
        return make_api_call("/users/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(username))) + "/statistics","GET");
    }
    pplx::task<json::value> getUserFavorites(const std::string& username) {
        return make_api_call("/users/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(username))) + "/favorites","GET");
    }
    pplx::task<json::value> getUserUpdates(const std::string& username) {
        return make_api_call("/users/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(username))) + "/userupdates","GET");
    }
    pplx::task<json::value> getUserAbout(const std::string& username) {
        return make_api_call("/users/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(username))) + "/about","GET");
    }
    pplx::task<json::value> getUserHistory(const std::string& username) {
        return make_api_call("/users/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(username))) + "/history","GET");
    }
    pplx::task<json::value> getUserFriends(const std::string& username) {
        return make_api_call("/users/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(username))) + "/friends","GET");
    }
    pplx::task<json::value> getUserReviews(const std::string& username) {
        return make_api_call("/users/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(username))) + "/reviews","GET");
    }
    pplx::task<json::value> getUserRecommendations(const std::string& username) {
        return make_api_call("/users/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(username))) + "/recommendations","GET");
    }
    pplx::task<json::value> getUserClubs(const std::string& username) {
        return make_api_call("/users/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(username))) + "/clubs","GET");
    }
    pplx::task<json::value> getUserExternal(const std::string& username) {
        return make_api_call("/users/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(username))) + "/external","GET");
    }
/// User Anime lists and User Manga lists have been discontinued since May 1st, 2022. https://docs.google.com/document/d/1-6H-agSnqa8Mfmw802UYfGQrceIEnAaEh4uCXAPiX5A
    pplx::task<json::value> getUserAnimelist(const std::string& username) {
        return make_api_call("/users/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(username))) + "/animelist","GET");
    }
    pplx::task<json::value> getUserMangaList(const std::string& username) {
        return make_api_call("/users/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(username))) + "/mangalist","GET");
    }

    pplx::task<json::value> getSeasonNow(int page = 0,int limit = 0,const std::string& filter = "", bool sfw=false,bool continuing=false,bool unapproved=false) {
        std::map<std::string, std::string> params;
        if (page > 0) params["page"] = std::to_string(page);
        if (limit > 0) params["limit"] = std::to_string(limit);
        if (!filter.empty()) params["filter"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(filter)));
        if (continuing) params["continuing"] = "";
        if (sfw) params["sfw"] = "";
        if (unapproved) params["unapproved"] = "";
        std::string url = "/seasons/now" + build_query_params(params);
        return make_api_call(url, "GET");
    }

    pplx::task<json::value> getSeason(int year= 0,int season= 0,int page = 0,int limit = 0,const std::string& filter = "", bool sfw=false,bool continuing=false,bool unapproved=false) {
        std::map<std::string, std::string> params;
        if (year > 0) params["year"] = std::to_string(year);
        if (season > 0) params["season"] = std::to_string(season);
        if (page > 0) params["page"] = std::to_string(page);
        if (limit > 0) params["limit"] = std::to_string(limit);
        if (!filter.empty()) params["filter"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(filter)));
        if (continuing) params["continuing"] = "";
        if (sfw) params["sfw"] = "";
        if (unapproved) params["unapproved"] = "";
        std::string url = "/seasons/" + std::to_string(year) + "/" + std::to_string(season) + build_query_params(params);
        return make_api_call(url, "GET");
    }

    pplx::task<json::value> getSeasonsList() {
        return make_api_call("/seasons", "GET");
    }

    pplx::task<json::value> getSeasonUpcoming(int page = 0,int limit = 0,const std::string& filter = "", bool sfw=false,bool continuing=false,bool unapproved=false) {
        std::map<std::string, std::string> params;
        if (page > 0) params["page"] = std::to_string(page);
        if (limit > 0) params["limit"] = std::to_string(limit);
        if (!filter.empty()) params["filter"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(filter)));
        if (continuing) params["continuing"] = "";
        if (sfw) params["sfw"] = "";
        if (unapproved) params["unapproved"] = "";
        std::string url = "/seasons/upcoming" + build_query_params(params);
        return make_api_call(url, "GET");
    }
    pplx::task<json::value> getAnimeSearch(int page = 0,int limit = 0,const std::string& q = "", const std::string& type = "",const std::string& filter = "", const std::string& rating = "",bool sfw = false) {
        std::map<std::string, std::string> params;
        if (page > 0) params["page"] = std::to_string(page);
        if (limit > 0) params["limit"] = std::to_string(limit);
        if (!type.empty()) params["type"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(type)));
        if (!filter.empty()) params["filter"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(filter)));
        if (!rating.empty()) params["rating"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(rating)));
        if (sfw) params["sfw"] = "";
        std::string url = "/top/anime"+ build_query_params(params);
        return make_api_call(url, "GET");
    }
    pplx::task<json::value> getTopManga(int page = 0,int limit = 0,const std::string& q = "", const std::string& type = "",const std::string& filter = "") {
        std::map<std::string, std::string> params;
        if (page > 0) params["page"] = std::to_string(page);
        if (limit > 0) params["limit"] = std::to_string(limit);
        if (!type.empty()) params["type"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(type)));
        if (!filter.empty()) params["filter"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(filter)));
        std::string url = "/top/manga"+ build_query_params(params);
        return make_api_call(url, "GET");
    }
    pplx::task<json::value> getTopPeople(int page = 0,int limit = 0) {
        std::map<std::string, std::string> params;
        if (page > 0) params["page"] = std::to_string(page);
        if (limit > 0) params["limit"] = std::to_string(limit);
        std::string url = "/top/people"+ build_query_params(params);
        return make_api_call(url, "GET");
    }
    pplx::task<json::value> getTopCharacters(int page = 0,int limit = 0) {
        std::map<std::string, std::string> params;
        if (page > 0) params["page"] = std::to_string(page);
        if (limit > 0) params["limit"] = std::to_string(limit);
        std::string url = "/top/characters";
        return make_api_call(url, "GET");
    }
    pplx::task<json::value> getTopReviews(int page = 0,int limit = 0,const std::string& q = "", const std::string& type = "",bool preliminary = false,bool spoilers = false) {
        std::map<std::string, std::string> params;
        if (page > 0) params["page"] = std::to_string(page);
        if (limit > 0) params["limit"] = std::to_string(limit);
        if (!type.empty()) params["type"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(type)));
        if (preliminary) params["preliminary"] = preliminary;
        if (spoilers) params["spoilers"] = spoilers;
        std::string url = "/top/reviews"+ build_query_params(params);
        return make_api_call(url, "GET");
    }
    pplx::task<json::value> getWatchRecentPromos(int page = 0) {
        std::map<std::string, std::string> params;
        if (page > 0) params["page"] = std::to_string(page);
        std::string url = "/watch/promos"+ build_query_params(params);
        return make_api_call(url, "GET");
    }
    pplx::task<json::value> getWatchPopularPromos() {
        return make_api_call("/watch/promos/popular","GET");
    }
    pplx::task<json::value> getWatchPopularEpisodes() {
        return make_api_call("/watch/episodes/popular","GET");
    }
    pplx::task<json::value> getWatchRecentEpisodes() {
        return make_api_call("/watch/episodes","GET");
    }
};

#endif
