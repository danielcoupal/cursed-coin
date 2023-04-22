class Ring {
    private:
        double thick;
        double r_out;
        double r_inner;
        double z_rad;

    public:
        Ring(double thick, double r_out, double r_inner);
        bool is_rim_surface(double x, double y, double z, double z_rad);
        bool is_rim_edge(double x, double y, double z, double z_rad);
        bool is_rim_side(double x, double y, double z, double z_rad);
};

