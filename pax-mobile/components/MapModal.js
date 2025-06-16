import React from "react";
import {
    Modal,
    View,
    Image,
    TouchableOpacity,
    TouchableWithoutFeedback,
    Keyboard,
    StyleSheet,
    Text,
    AccessibilityInfo,
} from "react-native";

const MapModal = ({ isVisible, onClose, mapImage, imageDescription }) => {
    return (
        <Modal
            visible={isVisible}
            animationType="fade"
            transparent={true}
            onRequestClose={onClose}
            accessible={true}
            accessibilityViewIsModal={true}
            accessibilityLabel="Kartvy öppen"
        >
            <TouchableWithoutFeedback onPress={onClose} accessible={false}>
                <View style={styles.modalContainer}>
                    <TouchableWithoutFeedback accessible={false}>
                        <View
                            style={styles.modalContent}
                            accessibilityRole="dialog"
                            accessibilityLabel="Modal med karta"
                        >
                            <Image
                                source={mapImage}
                                style={styles.mapImage}
                                accessibilityLabel={imageDescription || "Karta över rummet"}
                                accessible={true}
                            />
                            {imageDescription && (
                                <Text
                                    style={styles.imageDescription}
                                    accessibilityLabel={`Beskrivning: ${imageDescription}`}
                                    accessible={true}
                                >
                                    {imageDescription}
                                </Text>
                            )}
                            <TouchableOpacity
                                onPress={onClose}
                                style={styles.closeButton}
                                accessibilityRole="button"
                                accessibilityLabel="Stäng karta"
                                accessible={true}
                            >
                                <Text style={styles.closeButtonText}>Stäng</Text>
                            </TouchableOpacity>
                        </View>
                    </TouchableWithoutFeedback>
                </View>
            </TouchableWithoutFeedback>
        </Modal>
    );
};

const styles = StyleSheet.create({
    modalContainer: {
        flex: 1,
        justifyContent: "center",
        alignItems: "center",
        backgroundColor: "rgba(0, 0, 0, 0.51)",
    },
    modalContent: {
        backgroundColor: "rgba(222, 247, 194, 0.99)",
        padding: 15,
        borderRadius: 10,
        alignItems: "center",
        width: "90%",
        maxHeight: "70%",
        marginTop: "1%",
    },
    mapImage: {
        width: "100%",
        height: 300,
        resizeMode: "contain",
    },
    imageDescription: {
        marginTop: 10,
        color: "#000", // korrekt färgkod
        fontSize: 14,
        textAlign: "center",
    },
    closeButton: {
        marginTop: 15,
        padding: 10,
        backgroundColor: "#7DBA6A",
        borderRadius: 8,
    },
    closeButtonText: {
        color: "#fff",
        fontWeight: "600",
        fontSize: 16,
    },
});

export default MapModal;
