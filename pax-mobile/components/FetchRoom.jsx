import React, { useEffect, useState } from "react";
import {
  View,
  Text,
  ActivityIndicator,
  StyleSheet,
  FlatList,
  TouchableOpacity,
  Modal,
  Pressable,
  ScrollView,
  Image,
  Alert,
} from "react-native";
import { useTheme } from "../theme/ThemeContext";
import BookingModal from "./BookingModal";
import FavoriteButton from "../components/favoriteButton";
import AsyncStorage from "@react-native-async-storage/async-storage";

const FetchRoom = ({ favorites, setFavorites, setAllRooms }) => {
  const { theme } = useTheme();
  const styles = createStyles(theme);

  const [rooms, setRooms] = useState([]);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState(null);
  const [selectedRoom, setSelectedRoom] = useState(null);
  const [modalVisible, setModalVisible] = useState(false);
  const [bookingModalVisible, setBookingModalVisible] = useState(false);
  const [imageError, setImageError] = useState(false);

  useEffect(() => {
    const fetchRooms = async () => {
      try {
        const response = await fetch("https://paxdb.vercel.app/rooms");
        const data = await response.json();
        const availableRooms = (data || []).filter((room) => room.available);
        setRooms(availableRooms);
        setAllRooms(data || []);
      } catch (err) {
        setError("Något gick fel vid hämtning av rum.");
      } finally {
        setLoading(false);
      }
    };

    fetchRooms();
  }, []);

  const toggleFavorite = async (roomId) => {
    const updated = {
      ...favorites,
      [roomId]: !favorites[roomId],
    };
    setFavorites(updated);
    try {
      await AsyncStorage.setItem("favorites", JSON.stringify(updated));
    } catch (e) {
      console.error("Kunde inte uppdatera favoriter", e);
    }
  };

  const openRoomModal = (room) => {
    setSelectedRoom(null);
    setTimeout(() => {
      setSelectedRoom(room);
      setImageError(false);
      setModalVisible(true);
    }, 10);
  };

  const closeModal = () => {
    setModalVisible(false);
    setSelectedRoom(null);
  };

  const handleBookingSuccess = (roomId) => {
    setRooms((prev) => prev.filter((room) => room.id !== roomId));
    setBookingModalVisible(false);
    setSelectedRoom(null);
    Alert.alert("Bokning lyckades", "Rummet är nu bokat.");
  };

  if (loading) {
    return <ActivityIndicator size="large" color={theme.accent} />;
  }

  if (error) {
    return <Text style={styles.error}>{error}</Text>;
  }

  return (
    <View style={{ flex: 1, paddingBottom: 13 }}>
      {rooms.length === 0 ? (
        <Text style={styles.noRooms}>Inga lediga rum hittades.</Text>
      ) : (
        <FlatList
          data={rooms}
          keyExtractor={(item) => item.id.toString()}
          numColumns={2}
          columnWrapperStyle={{
            justifyContent: "space-between",
            paddingHorizontal: 10,
          }}
          contentContainerStyle={{ paddingTop: 10 }}
          renderItem={({ item }) => (
            <TouchableOpacity
              style={styles.roomItem}
              onPress={() => openRoomModal(item)}
              accessible={true}
              accessibilityRole="button"
              accessibilityLabel={`Rum ${item.name}, tryck för mer information`}
            >
              <Text style={styles.roomName}>{item.name}</Text>
            </TouchableOpacity>
          )}
        />
      )}

      <Modal
        visible={modalVisible}
        animationType="slide"
        transparent={true}
        onRequestClose={closeModal}
      >
        <View style={styles.modalBackground}>
          <View style={styles.modalContent}>
            {selectedRoom && (
              <ScrollView>
                {selectedRoom.img && !imageError ? (
                  <Image
                    source={{ uri: selectedRoom.img }}
                    style={styles.roomImage}
                    resizeMode="cover"
                    onError={() => setImageError(true)}
                    accessible={true}
                    accessibilityLabel={`Bild på rummet ${selectedRoom.name}`}
                  />
                ) : (
                  <Text style={styles.imageErrorText}>
                    Bilden kunde inte laddas.
                  </Text>
                )}

                <Text style={styles.roomName}>{selectedRoom.name}</Text>
                <Text style={styles.roomDescription}>
                  {selectedRoom.description}
                </Text>
                <Text style={styles.roomDetail}>
                  Våning: {selectedRoom.floor}
                </Text>
                <Text style={styles.roomDetail}>
                  Stolar: {selectedRoom.chairs}
                </Text>
                <Text style={styles.roomDetail}>
                  Luftkvalitet: {selectedRoom.air_quality}
                </Text>
                <Text style={styles.roomDetail}>
                  Skärm: {selectedRoom.screen ? "Ja" : "Nej"} | Whiteboard:{" "}
                  {selectedRoom.whiteboard ? "Ja" : "Nej"} | Projektor:{" "}
                  {selectedRoom.projector ? "Ja" : "Nej"}
                </Text>

                <FavoriteButton
                  isFavorite={favorites[selectedRoom.id]}
                  onToggle={() => toggleFavorite(selectedRoom.id)}
                />

                <Pressable
                  onPress={() => {
                    setBookingModalVisible(true);
                    setModalVisible(false);
                  }}
                  style={styles.bookButton}
                  accessible={true}
                  accessibilityRole="button"
                  accessibilityLabel="Boka rummet"
                >
                  <Text style={styles.bookButtonText}>Boka</Text>
                </Pressable>

                <Pressable
                  onPress={closeModal}
                  style={styles.closeButton}
                  accessible={true}
                  accessibilityRole="button"
                  accessibilityLabel="Stäng detaljer om rum"
                >
                  <Text style={styles.closeButtonText}>Stäng</Text>
                </Pressable>
              </ScrollView>
            )}
          </View>
        </View>
      </Modal>

      <BookingModal
        visible={bookingModalVisible}
        onClose={() => setBookingModalVisible(false)}
        room={selectedRoom}
        onBookingSuccess={handleBookingSuccess}
        theme={theme}
      />
    </View>
  );
};

const createStyles = (theme) =>
  StyleSheet.create({
    roomItem: {
      backgroundColor: theme.card,
      borderRadius: 10,
      marginBottom: 10,
      flex: 1,
      marginHorizontal: 5,
    },
    roomName: {
      fontSize: 16,
      fontWeight: "350",
      color: theme.textPrimary,
      marginTop: 5,
      marginBottom: 2,
    },
    roomDescription: {
      fontSize: 14,
      color: theme.textPrimary,
      marginVertical: 4,
    },
    roomDetail: {
      fontSize: 13,
      color: theme.textPrimary,
    },
    noRooms: {
      fontSize: 16,
      color: theme.textPrimary,
      fontStyle: "italic",
      textAlign: "center",
      marginTop: 20,
    },
    error: {
      color: "red",
      fontSize: 16,
      textAlign: "center",
      marginTop: 20,
    },
    modalBackground: {
      flex: 1,
      backgroundColor: "rgba(0,0,0,0.5)",
      justifyContent: "center",
      alignItems: "center",
    },
    modalContent: {
      backgroundColor: theme.background,
      padding: 20,
      borderRadius: 20,
      width: "85%",
      maxHeight: "80%",
    },
    roomImage: {
      width: "100%",
      height: 180,
      borderRadius: 10,
      marginBottom: 10,
    },
    imageErrorText: {
      fontSize: 14,
      fontStyle: "italic",
      color: theme.textSecondary,
      marginBottom: 10,
      textAlign: "center",
    },
    closeButton: {
      marginTop: 20,
      backgroundColor: theme.textSecondary,
      padding: 10,
      borderRadius: 10,
      alignItems: "center",
    },
    closeButtonText: {
      color: "#000",
      fontWeight: "600",
    },
    bookButton: {
      marginTop: 20,
      backgroundColor: theme.textSecondary,
      padding: 10,
      borderRadius: 10,
      alignItems: "center",
    },
    bookButtonText: {
      color: "#000",
      fontWeight: "600",
    },
  });

export default FetchRoom;
