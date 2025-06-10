import React, { useState, useEffect } from "react";
import {
  View,
  StyleSheet,
  SafeAreaView,
  StatusBar,
  Platform,
  Text,
  Pressable,
  FlatList,
  Modal,
  ScrollView,
  Image,
} from "react-native";
import { useTheme } from "../theme/ThemeContext";
import ThemeToggleTabButton from "../components/ThemeToggleTabButton";
import MapModal from "../components/MapModal";
import FetchRoom from "../components/FetchRoom";
import AsyncStorage from "@react-native-async-storage/async-storage";
import BookingModal from "../components/BookingModal";

const Booking = () => {
  const { theme, isDark } = useTheme();
  const styles = createStyles(theme);

  const [isModalVisible, setIsModalVisible] = useState(false);
  const [favorites, setFavorites] = useState({});
  const [allRooms, setAllRooms] = useState([]);

  const [selectedRoom, setSelectedRoom] = useState(null);
  const [bookingModalVisible, setBookingModalVisible] = useState(false);

  const toggleModal = () => {
    setIsModalVisible(!isModalVisible);
  };

  useEffect(() => {
    const loadFavorites = async () => {
      try {
        const stored = await AsyncStorage.getItem("favorites");
        setFavorites(JSON.parse(stored) || {});
      } catch (e) {
        console.error("Kunde inte läsa favoriter", e);
      }
    };

    loadFavorites();
  }, []);

  const favoriteRoomList = allRooms.filter((room) => favorites[room.id]);

  const openRoomModal = (room) => {
    setSelectedRoom(room);
    setBookingModalVisible(true);
  };

  const handleBookingSuccess = (roomId) => {
    setAllRooms((prev) => prev.filter((room) => room.id !== roomId));
    setBookingModalVisible(false);
    setSelectedRoom(null);
  };

  return (
    <SafeAreaView style={styles.safeArea}>
      <StatusBar
        barStyle={isDark ? "light-content" : "dark-content"}
        backgroundColor={theme.headerBackground}
      />
      <View style={styles.container}>
        <View style={styles.toggleButtonContainer}>
          <ThemeToggleTabButton />
        </View>
        <Text style={styles.bookingtext}>Boka ett rum</Text>

        <View style={styles.cardWrapper}>
          <View style={styles.card}>
            <Text style={styles.title}>Lediga rum:</Text>
            <FetchRoom
              favorites={favorites}
              setFavorites={setFavorites}
              setAllRooms={setAllRooms}
              onRoomPress={openRoomModal}
            />
          </View>

          <Pressable style={styles.pressableButton} onPress={toggleModal}>
            <Text style={styles.pressableText}>Kartöversikt</Text>
          </Pressable>

          <View style={styles.card2}>
            <Text style={styles.title}>Favoritrum:</Text>
            {favoriteRoomList.length === 0 ? (
              <Text style={{ color: theme.textSecondary, marginTop: 10 }}>
                Inga favoriter ännu.
              </Text>
            ) : (
              <FlatList
                data={favoriteRoomList}
                keyExtractor={(item) => item.id.toString()}
                renderItem={({ item }) => (
                  <Pressable onPress={() => openRoomModal(item)}>
                    <Text
                      style={{
                        color: theme.textPrimary,
                        fontSize: 16,
                        marginTop: 8,
                      }}
                    >
                      ★ {item.name}
                    </Text>
                  </Pressable>
                )}
              />
            )}
          </View>
        </View>
      </View>

      <MapModal
        isVisible={isModalVisible}
        onClose={toggleModal}
        mapImage={require("../assets/maps/karta-pax_med-nr.png")}
        imageDescription={
          "1: Norra Berget  2: Avenyn  3: Slottsskogen \n4: Liseberg 5: Stenstaden  6: Möllevångstorget  \n7: Turning Torso"
        }
      />

      <BookingModal
        visible={bookingModalVisible}
        onClose={() => setBookingModalVisible(false)}
        room={selectedRoom}
        onBookingSuccess={handleBookingSuccess}
        theme={theme}
      />
    </SafeAreaView>
  );
};

const createStyles = (theme) =>
  StyleSheet.create({
    safeArea: {
      flex: 1,
      backgroundColor: theme.background,
      paddingTop: Platform.OS === "android" ? StatusBar.currentHeight : 0,
    },
    container: {
      flex: 1,
      backgroundColor: theme.background,
      alignItems: "center",
      paddingTop: 80,
    },
    toggleButtonContainer: {
      position: "absolute",
      top: 10,
      left: 10,
      zIndex: 1,
    },
    bookingtext: {
      color: theme.card,
      fontFamily: "NunitoSans",
      fontSize: 30,
      fontWeight: "600",
      marginBottom: 8,
      letterSpacing: 0.5,
      textTransform: "uppercase",
    },
    cardWrapper: {
      width: "100%",
      alignItems: "center",
      marginTop: 16,
    },
    card: {
      backgroundColor: theme.card,
      height: 217,
      width: "90%",
      padding: 16,
      borderRadius: 30,
      marginVertical: 12,
      elevation: 3,
    },
    card2: {
      backgroundColor: theme.card,
      height: 217,
      width: "90%",
      padding: 16,
      borderRadius: 30,
      marginVertical: 12,
      elevation: 3,
    },
    title: {
      color: theme.textPrimary,
      fontFamily: "NunitoSans",
      fontSize: 20,
      fontWeight: "500",
    },
    pressableButton: {
      backgroundColor: theme.primary,
      width: "90%",
      height: 60,
      justifyContent: "center",
      alignItems: "center",
      borderRadius: 30,
      marginVertical: 12,
      elevation: 3,
      margin: 10,
      padding: 20,
      backgroundColor: theme.card,
      borderRadius: 20,
      textAlign: "right",
      marginRight: 10,
      elevation: 3,
    },
    pressableText: {
      color: theme.textPrimary,
      fontFamily: "NunitoSans",
      fontSize: 18,
      fontWeight: "600",
      textTransform: "uppercase",
    },
  });

export default Booking;
